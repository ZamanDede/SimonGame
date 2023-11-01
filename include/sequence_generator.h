#ifndef SEQUENCE_GENERATOR_H
#define SEQUENCE_GENERATOR_H

#include <stdint.h> // Include standard integer type definitions

// Pre-defined constants for LFSR operations
#define MASK 0xE2023CAB
#define SEED 0x11357428

// Function Prototypes

/**
 * @brief Initialize the Linear Feedback Shift Register (LFSR) with predefined seed.
 */
void initializeLFSR(void);

/**
 * @brief Extend the sequence produced by the LFSR.
 */
void extendSequence(void);

/**
 * @brief Reset the generator to its initial state.
 */
void resetGenerator(void);

/**
 * @brief Get the current state of the LFSR.
 *
 * @return Current state of LFSR as a 32-bit unsigned integer.
 */
uint32_t getLFSRState(void);

/**
 * @brief Set a new state for the LFSR.
 *
 * @param newState The new state to set for the LFSR.
 */
void setLFSRState(uint32_t newState);

#endif
