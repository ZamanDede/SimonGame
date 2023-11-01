#include "high_scores.h"
#include "uart.h"
#include <stdlib.h>
#include <stdio.h>

static HighScoreEntry high_scores[MAX_HIGH_SCORES];

// Initialize all high scores to 0
void high_scores_initialize(void)
{
    for (uint8_t i = 0; i < MAX_HIGH_SCORES; i++)
    {
        for (uint8_t j = 0; j < NAME_LENGTH; j++)
        {
            high_scores[i].name[j] = 0;
        }
        high_scores[i].score = 0;
    }
}

// String copy function with a limit
void my_strncpy(char *dest, const char *src, uint8_t n)
{
    uint8_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
}

// Check if the provided score is a top score
uint8_t high_scores_is_top_score(uint8_t score)
{
    return score > high_scores[MAX_HIGH_SCORES - 1].score;
}

// Add a new score to the list, maintaining order
void high_scores_add(const char *name, uint8_t score)
{
    int8_t insert_position = -1;

    for (int8_t i = MAX_HIGH_SCORES - 1; i >= 0; i--)
    {
        if (score > high_scores[i].score)
            insert_position = i;
        else
            break;
    }

    if (insert_position != -1)
    {
        for (int8_t i = MAX_HIGH_SCORES - 1; i > insert_position; i--)
        {
            my_strncpy(high_scores[i].name, high_scores[i - 1].name, NAME_LENGTH);
            high_scores[i].score = high_scores[i - 1].score;
        }
        my_strncpy(high_scores[insert_position].name, name, NAME_LENGTH);
        high_scores[insert_position].score = score;
    }
}

// Display all high scores
void high_scores_display(void)
{
    char buffer[30];

    for (uint8_t i = 0; i < MAX_HIGH_SCORES; i++)
    {
        if (high_scores[i].score == 0)
            break;

        sprintf(buffer, "%s %u\n", high_scores[i].name, high_scores[i].score);
        uart_puts(buffer);
    }
}
