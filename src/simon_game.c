#include "simon_game.h"
#include "uart.h"
#include "sequence_generator.h"
#include "buttons.h"
#include "display_macros.h"
#include "display_output.h"
#include "high_scores.h"
#include "buzzer.h"
#include <avr/interrupt.h>
#include <stdio.h>

static uint8_t simon_sequence[MAX_SEQUENCE_LENGTH];
uint8_t user_sequence[MAX_SEQUENCE_LENGTH];
static uint8_t current_sequence_length;
uint8_t user_input_index;
volatile uint8_t current_display_index;
uint16_t elapsed_time;
uint16_t delay_length;
volatile uint8_t failure_phase;
uint8_t user_score;
uint8_t is_counting;
char name_buffer[NAME_LENGTH];
uint8_t name_buffer_idx;
uint8_t is_5_ms;

GameState current_state;
GameState previous_state;

const uint8_t digit_patterns[10] = {
    DISP_0, DISP_1, DISP_2, DISP_3, DISP_4,
    DISP_5, DISP_6, DISP_7, DISP_8, DISP_9};

void extend_sequence(void)
{
    extendSequence();                             // Update the LFSR state
    uint8_t generated_value = getLFSRState() % 4; // Get a value between 0 and 3
    simon_sequence[current_sequence_length++] = generated_value;
}

void simon_game_initialize(void)
{
    compute_delay_length();
    current_sequence_length = 0;
    current_display_index = 0;
    user_input_index = 0;
    elapsed_time = 0;
    failure_phase = 1;
    is_counting = 1;
    name_buffer_idx = 0;
    current_state = INIT;
    previous_state = INIT;
}

uint8_t compare_sequences(void)
{
    for (uint8_t i = 0; i < user_input_index; i++)
    {
        if (simon_sequence[i] != user_sequence[i])
            return i; // sequences are not the same
    }
    return -1; // sequences are the same
}

void compute_delay_length()
{
    uint8_t adc_val = ADC0.RESULT;

    uint32_t temp_delay = (uint32_t)adc_val * 448 + (uint32_t)(adc_val % 64) * 7;

    delay_length = MIN_DELAY + temp_delay / 64;

}


void simon_game_update(void)
{
    switch (current_state)
    {
    case INIT:
        simon_game_initialize();
        current_state = SIMON_PLAYS;
        break;

    case WAIT:
        TCB0.CTRLA |= TCB_ENABLE_bm;
        if (elapsed_time > delay_length)
        {
            if (is_5_ms)
            {
                high_scores_add(name_buffer, user_score);
                uart_puts("\n");
                is_5_ms = 0;
                current_state = INIT;
                high_scores_display();
            }
            else
            {
                buzzer_off();
                update_display(DISP_OFF, DISP_OFF);
                current_state = previous_state;
            }
            TCB0.CTRLA &= ~TCB_ENABLE_bm; // Disable the timer
            elapsed_time = 0;
            is_counting = 0;
        }
        break;
    case HALFWAY_PAUSE:
        TCB0.CTRLA |= TCB_ENABLE_bm;
        if (elapsed_time >= (delay_length >> 1))
        {
            buzzer_off();
            update_display(DISP_OFF, DISP_OFF);
            previous_state = SIMON_PLAYS; // When the WAIT state is done, it should go back to SIMON_PLAYS
            TCB0.CTRLA &= ~TCB_ENABLE_bm; // Disable the timer
            current_state = WAIT;         // Move to WAIT
        }
        break;

    case SIMON_PLAYS:
        if (is_counting)
        {
            extend_sequence();
        }
        if (current_display_index < current_sequence_length)
        {
            switch (simon_sequence[current_display_index])
            {
            case 0:
                buzzer_on(0);
                update_display(DISP_VERT_BAR_LEFT, DISP_OFF);
                break;

            case 1:
                buzzer_on(1);
                update_display(DISP_VERT_BAR_RIGHT, DISP_OFF);
                break;

            case 2:
                buzzer_on(2);
                update_display(DISP_OFF, DISP_VERT_BAR_LEFT);
                break;

            case 3:
                buzzer_on(3);
                update_display(DISP_OFF, DISP_VERT_BAR_RIGHT);
                break;

            default:
                break;
            }
            previous_state = SIMON_PLAYS;
            current_state = HALFWAY_PAUSE;
            current_display_index++; // Move to the next tone
        }
        else
        {
            is_counting = 1;
            current_display_index = 0;  // Reset for the next sequence
            current_state = USER_PLAYS; // Transition to USER_PLAYS
            break;
        }

        break;

    case USER_PLAYS:
        compute_delay_length();
        // Enable uart_input
        USART0.CTRLA |= USART_RXCIE_bm;
        // Read/React to user input from buttons
        button_state_machine_execute();

        int8_t divergence_index = compare_sequences();

        if (divergence_index >= 0) // There was a mistake
        {
            // Disable uart_input
            USART0.CTRLA &= ~USART_RXCIE_bm;
            user_input_index = 0;
            current_state = FAILURE;
        }
        else if (user_input_index && (user_input_index == current_sequence_length)) // The user successfully matched the entire sequence
        {
            // Disable uart_input
            USART0.CTRLA &= ~USART_RXCIE_bm;
            user_input_index = 0;
            current_state = SUCCESS;
        }
        // Else: the user is still in the middle of inputting the sequence. Do nothing and wait.

        break;

    case SUCCESS:
        if (is_counting)
        {
            uart_puts("SUCCESS\n");
            update_display(DISP_ON, DISP_ON);
            previous_state = SUCCESS; // Set where we'll return to after the wait
            current_state = WAIT;     // Move to the wait state
        }

        else if (user_input_index >= MAX_SEQUENCE_LENGTH)
        {
            is_counting = 1;
            current_state = UPDATE_SCORES;
        }
        else
        {
            char score_str[6];
            sprintf(score_str, "%u\n", current_sequence_length);
            uart_puts(score_str);
            is_counting = 1;
            current_state = SIMON_PLAYS;
        }
        break;

    case FAILURE:
        if (is_counting)
        {
            if (failure_phase == 1)
            {
                // Uart failure message
                uart_puts("GAME OVER\n");

                // Display the FAIL pattern
                update_display(DISP_SEG_G, DISP_SEG_G);
                previous_state = FAILURE; // Set the previous state to FAILURE
                current_state = WAIT;     // Transition to WAIT state
            }
            else if (failure_phase == 2)
            {
                // Calculate score
                user_score = (current_sequence_length - 1) > 0 ? (current_sequence_length - 1) : 0;
                // Uart display score
                char score_str[6];
                sprintf(score_str, "%u\n", user_score);
                uart_puts(score_str);

                // Display the user's score
                uint8_t tens = user_score / 10;
                uint8_t ones = user_score % 10;
                update_display(DISP_OFF, DISP_OFF);
                update_display(digit_patterns[tens], digit_patterns[ones]);

                previous_state = FAILURE; // Set the previous state to FAILURE
                current_state = WAIT;     // Transition to WAIT state
            }
            else if (failure_phase == 3)
            {
                failure_phase = 1; // Reset failure phase
                current_state = UPDATE_SCORES;
            }
        }
        else // This handles the phase increment logic after the wait
        {
            if (failure_phase < 3)
            {
                failure_phase++;         // Increment the failure phase
                current_state = FAILURE; // Return to FAILURE state to start the next phase
                is_counting = 1;
            }
        }
        break;

    case UPDATE_SCORES:
        if (high_scores_is_top_score(user_score))
        {
            uart_puts("Enter name: ");
            is_5_ms = 1;
            USART0.CTRLA |= USART_RXCIE_bm;
            current_state = WAIT;
        }
        else
        {
            high_scores_display();
            current_state = INIT;
        }
        break;

    default:
        current_state = SIMON_PLAYS;
        break;
    }
}

// ISR for playback_delay
ISR(TCB0_INT_vect)
{
    // Count in 1ms
    elapsed_time++;

    if (is_5_ms)
        delay_length = 5000;
    else
        // Compute delay using the new function
        compute_delay_length();

    TCB0.INTFLAGS |= TCB_CAPT_bm;
}
