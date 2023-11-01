#include <avr/io.h>
#include <stdint.h>
#include "buzzer.h"

#define TONE1_PER 10374
#define TONE2_PER 12333
#define TONE3_PER 7785
#define TONE4_PER 20812

static uint16_t periods[4] = {TONE1_PER, TONE2_PER, TONE3_PER, TONE4_PER};

void buzzer_on(const uint8_t tone)
{
    TCA0.SINGLE.PERBUF = periods[tone];
    TCA0.SINGLE.CMP0BUF = periods[tone] / 2;
}

void buzzer_off(void)
{
    TCA0.SINGLE.CMP0BUF = 0;
}

void increase_frequency(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        periods[i] >>= 1;  // Divide by 2 to increase the frequency by an octave
    }
}

void decrease_frequency(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        periods[i] <<= 1;  // Multiply by 2 to decrease the frequency by an octave
    }
}

void reset_tones(void)
{
    periods[0] = TONE1_PER;
    periods[1] = TONE2_PER;
    periods[2] = TONE3_PER;
    periods[3] = TONE4_PER;
}
