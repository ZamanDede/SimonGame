#ifndef HIGH_SCORES_H
#define HIGH_SCORES_H

#include <stdint.h>

#define MAX_HIGH_SCORES 5  // Maximum number of high scores to store
#define NAME_LENGTH 10      // Maximum length for a player's name

// External buffer and index for name input
extern char name_buffer[NAME_LENGTH];       // Buffer to store the current name
extern uint8_t name_buffer_idx;             // Index to track position within the buffer

// Struct to represent an individual high score entry
typedef struct
{
    char name[NAME_LENGTH];  // Player's name
    uint8_t score;           // Player's score
} HighScoreEntry;

// Function to initialize the high scores system
void high_scores_initialize(void);

/**
 * Add a new score to the high scores list.
 *
 * @param name Player's name.
 * @param score Player's score.
 */
void high_scores_add(const char* name, uint8_t score);

// Function to display the high scores
void high_scores_display(void);

/**
 * Check if a score qualifies as a top score.
 *
 * @param score Score to be checked.
 * @return 1 if top score, 0 otherwise.
 */
uint8_t high_scores_is_top_score(uint8_t score);

// Function to prompt the user to enter their name for the high score
void high_scores_prompt_for_name(void);

#endif
