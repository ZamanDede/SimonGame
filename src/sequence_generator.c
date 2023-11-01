#include "sequence_generator.h"

static uint32_t LFSR_STATE;

void initializeLFSR(void)
{
    LFSR_STATE = SEED;
}

void setLFSRState(uint32_t newState)
{
    LFSR_STATE = newState;
}

uint32_t getLFSRState(void)
{
    return LFSR_STATE;
}


void extendSequence(void)
{
    uint32_t bit = LFSR_STATE & 0b1;  // Extract least significant bit
    LFSR_STATE >>= 1;                 // Right shift the LFSR state
    if (bit == 1)
    {
        LFSR_STATE ^= MASK;          // XOR with MASK if least significant bit was 1
    }
}

void resetGenerator(void)
{
    LFSR_STATE = SEED;  // Reset LFSR to seed
}
