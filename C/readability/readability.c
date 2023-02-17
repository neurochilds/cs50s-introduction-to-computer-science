#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// Count number of letters in text, ignoring all non-alphabetical characters
int count_letters(string text)
{
    int letters = 0;
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        if (isalpha(text[i]) != 0)
        {
            letters++;
        }
    }
    return letters;
}

// Count number of words in text, assuming any char followed by a space is a word, that text does not begin/start with a word
// and that there is never more than one space in a row.
int count_words(string text)
{
    int words = 1;
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        if (isspace(text[i]) != 0)
        {
            words++;
        }
    }
    return words;
}

// Count number of sentences in text, assuming all sentences end in either ".", "!", or "?".
int count_sentences(string text)
{
    int sentences = 0;
    char sentence_endings[3] = ".!?";
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (text[i] == sentence_endings[j])
            {
                sentences++;
            }
        }
    }
    return sentences;
}

// Compute Coleman-Liau index of text
float get_index(float letters, float words, float sentences)
{
    float letters_per_100_words = letters / words * 100;
    float sentences_per_100_words = sentences / words * 100;
    float index = 0.0588 * letters_per_100_words - 0.296 * sentences_per_100_words - 15.8;
    return index;
}

// Prompt user for text and return US grade level based on the Coleman-Liau index
int main(void)
{
    string text = get_string("Text: ");
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);
    int grade = round(get_index(letters, words, sentences));

    if (grade > 0 && grade < 16)
    {
        printf("Grade %i\n", grade);
        return 0;
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
        return 0;
    }
    printf("Before Grade 1\n");
    return 0;
}