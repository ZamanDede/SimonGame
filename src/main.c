#include "simon_game.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "initialisation.h"
#include "buzzer.h"
#include "display_output.h"
#include "display_macros.h"
#include "initialisation.h"
#include "sequence_generator.h"
#include "timer.h"
#include "high_scores.h"


void initialize_system(void)
{
    simon_game_initialize();
    high_scores_initialize();
    initializeLFSR();

    // Hardware initializations
    update_display(DISP_OFF, DISP_OFF);
    buzzer_off();
    button_init();
    spi_init();
    uart_init();
    pwm_init();
    timer_init();
    adc_init();
    compute_delay_length();

    // Configure output pin
    PORTB.OUTSET = PIN5_bm; // Setting the initial state of the pin
    PORTB.DIRSET = PIN5_bm; // Setting the pin as output

}

int main(void)
{
    // Setup Game
    cli();
    initialize_system();
    sei();

    while (1)
    {
        // Play Game
        simon_game_update();
    }

    return 0;
}
