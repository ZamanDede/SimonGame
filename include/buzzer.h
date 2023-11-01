#ifndef BUZZER_H
#define BUZZER_H

#include <avr/io.h>
#include <avr/interrupt.h>

// Activates the buzzer with a given tone
void buzzer_on(const uint8_t tone);

// Deactivates the buzzer
void buzzer_off(void);

// Increases the buzzer's frequency
void increase_frequency(void);

// Decreases the buzzer's frequency
void decrease_frequency(void);

// Resets the buzzer tones to default
void reset_tones(void);

#endif
