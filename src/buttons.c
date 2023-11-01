#include <avr/interrupt.h>
#include "buzzer.h"
#include "display_output.h"
#include "display_macros.h"
#include "timer.h"
#include "simon_game.h"

// States
typedef enum
{
    WAITB,
    TONE1,
    TONE2,
    TONE3,
    TONE4
} ButtonStateMachine;

// Global variables
volatile uint8_t pb_previous_state = 0xFF;
volatile uint8_t pb_current_state = 0xFF;
static ButtonStateMachine currentState = WAITB;

void button_state_machine_execute(void)
{
    pb_current_state = pb_debounced_state;
    uint8_t pb_falling_edge = (pb_previous_state ^ pb_current_state) & pb_previous_state;
    uint8_t pb_rising_edge = (pb_previous_state ^ pb_current_state) & pb_current_state;

    pb_previous_state = pb_current_state;

    // State machine for button press handling and related actions
    switch (currentState)
    {
    // Check for button press and move to corresponding state
    // Display and sound actions are done based on the button pressed
    case WAITB:
        if (pb_falling_edge & PIN4_bm)
        {
            currentState = TONE1;
            buzzer_on(0);
            update_display(DISP_VERT_BAR_LEFT, DISP_OFF);
        }
        else if (pb_falling_edge & PIN5_bm)
        {
            currentState = TONE2;
            buzzer_on(1);
            update_display(DISP_VERT_BAR_RIGHT, DISP_OFF);
        }
        else if (pb_falling_edge & PIN6_bm)
        {
            currentState = TONE3;
            buzzer_on(2);
            update_display(DISP_OFF, DISP_VERT_BAR_LEFT);
        }
        else if (pb_falling_edge & PIN7_bm)
        {
            currentState = TONE4;
            buzzer_on(3);
            update_display(DISP_OFF, DISP_VERT_BAR_RIGHT);
        }
        break;

    // Check if the button is released and move back to WAITB state
    case TONE1:
        if (pb_rising_edge & PIN4_bm)
        {
            user_sequence[user_input_index++] = 0;
            currentState = WAITB;
            buzzer_off();
            update_display(DISP_OFF, DISP_OFF);
        }
        break;
    case TONE2:
        if (pb_rising_edge & PIN5_bm)
        {
            user_sequence[user_input_index++] = 1;
            currentState = WAITB;
            buzzer_off();
            update_display(DISP_OFF, DISP_OFF);
        }
        break;
    case TONE3:
        if (pb_rising_edge & PIN6_bm)
        {
            user_sequence[user_input_index++] = 2;
            currentState = WAITB;
            buzzer_off();
            update_display(DISP_OFF, DISP_OFF);
        }
        break;
    case TONE4:
        if (pb_rising_edge & PIN7_bm)
        {
            user_sequence[user_input_index++] = 3;
            currentState = WAITB;
            buzzer_off();
            update_display(DISP_OFF, DISP_OFF);
        }
        break;
    default:
        currentState = WAITB;
        break;
    }
}
