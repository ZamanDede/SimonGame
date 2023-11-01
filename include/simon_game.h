#ifndef SIMON_GAME_H
#define SIMON_GAME_H

#include <stdint.h>

// Game related constants
#define MAX_SEQUENCE_LENGTH 20  // Maximum length of the Simon game sequence
#define ADC_MAX 255  // Maximum value for ADC readings
#define MIN_DELAY 250  // Minimum delay time in milliseconds
#define MAX_DELAY 2000  // Maximum delay time in milliseconds

// Enum to define possible game states
typedef enum
{
    INIT,           // Initial state
    WAIT,           // Waiting for user input
    HALFWAY_PAUSE,  // Halfway pause state in the game sequence
    SIMON_PLAYS,    // State where Simon device is playing the sequence
    USER_PLAYS,     // State where user is replicating the sequence
    SUCCESS,        // State indicating user successfully replicated sequence
    FAILURE,        // State indicating user failed to replicate sequence
    UPDATE_SCORES   // State to update the high scores
} GameState;

// Externally declared global variables related to the game state
extern uint8_t user_sequence[MAX_SEQUENCE_LENGTH];  // Stores the sequence the user has to replicate
extern uint8_t user_input_index;  // Current index of user input in the game sequence
extern uint8_t is_counting;       // Flag to check if game timer is counting
extern uint16_t elapsed_time;     // Time elapsed since last user input
extern uint16_t delay_length;     // Current delay length based on game progression
extern uint8_t user_score;        // Current score of the user
extern GameState current_state;   // Current game state
extern GameState previous_state;  // Previous game state for reference
extern uint8_t is_5_ms;           // 5ms time flag

// Function prototypes

/**
 * @brief Initialize the Simon game parameters.
 */
void simon_game_initialize(void);

/**
 * @brief Update the game state and handle user inputs and game progress.
 */
void simon_game_update(void);

/**
 * @brief Get the current score of the user.
 *
 * @return Current user score as an 8-bit unsigned integer.
 */
uint8_t simon_game_get_current_score(void);

/**
 * @brief Compute the delay length based on the current state of the game.
 */
void compute_delay_length(void);

#endif // SIMON_GAME_H
