#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

const unsigned int N = 25000;

unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    long sum = 0;
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        printf("Letter: %c\n", word[i]);
        int num = abs(toupper(word[i]) - 'A'); // Convert each character to a number, where A/a = 0, Zz = 25
        printf("Number: %i\n", num);
        sum += pow(num + i, 3); // Cube each number and add to sum
        printf("Sum: %li\n", sum);
    }
    printf("Sum: %li\n", sum);
    return sum % N; // Mod N caps the max index at N
}


int main(void)
{
    char *word = "abe's";
    hash(word);
}