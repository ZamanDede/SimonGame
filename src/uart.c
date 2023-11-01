#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"
#include "simon_game.h"
#include "buzzer.h"
#include "high_scores.h"
#include "sequence_generator.h"
#include <string.h>



char uart_getc(void)
{
    while (!(USART0.STATUS & USART_RXCIF_bm))
        ;
    return USART0.RXDATAL;
}

void uart_putc(char c)
{
    while (!(USART0.STATUS & USART_DREIF_bm))
        ;
    USART0.TXDATAL = c;
}

void uart_puts(char *ptr)
{
    while (*ptr != '\0')
    {
        uart_putc(*ptr);
        ptr++;
    }
}

void process_uart_data(char data)
{

    switch (data)
    {
    case '1':
    case 'q':
        user_sequence[user_input_index++] = 0;
        break;

    case '2':
    case 'w':
        user_sequence[user_input_index++] = 1;
        break;

    case '3':
    case 'e':
        user_sequence[user_input_index++] = 2;
        break;

    case '4':
    case 'r':
        user_sequence[user_input_index++] = 3;
        break;

    case ',':
    case 'k':
        increase_frequency();
        break;

    case '.':
    case 'l':
        decrease_frequency();
        break;

    case '0':
    case 'p':
        simon_game_initialize();
        reset_tones();
        break;

    case '9':
    case 'o':
    {
        char seed_string[9];
        for (int i = 0; i < 8; i++)
        {
            seed_string[i] = uart_getc();
        }
        seed_string[8] = '\0'; // Null-terminate
        uint32_t seed = strtoul(seed_string, 0, 16);
        setLFSRState(seed);
        break;
    }

    default:
        break;
    }
}

ISR(USART0_RXC_vect)
{
    char data = USART0.RXDATAL;

    if (data == '\r') // Ignore carriage return
        return;

    if (name_buffer_idx < (NAME_LENGTH - 1))
    {
        if (is_5_ms)
        {
            if (data == '\n')
            {
                name_buffer[name_buffer_idx] = '\0';
                high_scores_add(name_buffer, user_score);
                uart_puts("\n");
                high_scores_display();
                TCB0.CTRLA &= ~TCB_ENABLE_bm;  // Disable the timer
                USART0.CTRLA &= ~USART_RXCIE_bm; // Disable uart ISR
                current_state = INIT;
                is_5_ms = 0;
            }
            else
            {
                name_buffer[name_buffer_idx++] = data;
            }
            elapsed_time = 0;
        }
        else
        {
            process_uart_data(data);
        }
    }
    else
    {
        uart_puts("Name Limit Exceeded!");
    }
}
