#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>

#include "display_macros.h"
#include "display_output.h"


volatile uint8_t pb_debounced_state = 0xFF;

static void pb_debounce(void)
{

    static uint8_t vcount0 = 0, vcount1 = 0;

    uint8_t pb_sample = PORTA.IN;
    uint8_t pb_changed = (pb_sample ^ pb_debounced_state);

    vcount1 = (vcount1 ^ vcount0) & pb_changed;
    vcount0 = ~vcount0 & pb_changed;

    pb_debounced_state ^= (vcount0 & vcount1);

}

static void spi_write(void)
{
    static uint8_t current_side = 0;

    if (current_side)
    {
        SPI0.DATA = left_byte;
    }
    else
    {
        SPI0.DATA = right_byte;
    }

    // Toggle the current side
    current_side = !current_side;
}


ISR(TCB1_INT_vect)
{
    pb_debounce();
    spi_write();

    TCB1.INTFLAGS = TCB_CAPT_bm;
}
