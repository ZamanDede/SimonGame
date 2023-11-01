#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// External declarations for display bytes
extern volatile uint8_t left_byte;  // Left display byte
extern volatile uint8_t right_byte; // Right display byte

/**
 * Update the display with given bytes for left and right segments.
 *
 * @param left Byte to be displayed on the left segment.
 * @param right Byte to be displayed on the right segment.
 */
void update_display(const uint8_t left, const uint8_t right);

#endif
