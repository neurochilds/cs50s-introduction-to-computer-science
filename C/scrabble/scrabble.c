#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

int compute_score(string word)
{
    // Points assigned to each letter of the alphabet
    int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    int score = 0;
    int length = strlen(word);

    // Convert each char to uppercase, get its ASCII code and lookup its score. All non-letters are worth 0, hence are ignored.
    for (int i = 0; i < length; i++)
    {
        int ASCII_character = (int) toupper(word[i]);
        if (ASCII_character >= 65 && ASCII_character <= 90)
        {
            int index = ASCII_character - 65;
            score += POINTS[index];
        }
    }
    return score;
}

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
        return 0;
    }

    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
        return 0;
    }

    printf("Tie!\n");
    return 0;
}