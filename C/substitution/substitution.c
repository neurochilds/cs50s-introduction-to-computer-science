#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Encrypt text using the key. Characters A/a --> Z/z in text will be replaced with characters in key[0] --> key[25]
const char *encrypt(string text, string key)
{
    int length = strlen(text);
    for (int i = 0; i < length; i++)
    {
        char character = text[i];
        //if character is letter
        if (isalpha(character) != 0)
        {
            int ASCII_character = (int) toupper(character);
            int index = ASCII_character - 65; //A = 0, Z = 25
            int new_character = key[index];
            if (isupper(character) != 0)
            {
                text[i] = toupper(new_character);
            }
            else
            {
                text[i] = tolower(new_character);
            }
        }
    }
    return text;
}

// Check if key is valid.
bool key_is_valid(string key)
{
    int KEY_LENGTH = 26;
    if (strlen(key) != KEY_LENGTH)
    {
        printf("Key must be 26 letters long!\n");
        exit(1);
    }

    for (int i = 0; i < KEY_LENGTH; i++)
    {
        if (isalpha(key[i]) == 0)
        {
            printf("Key must contain only alphabetical characters!/n");
            exit(1);
        }
        for (int j = i + 1; j < KEY_LENGTH; j++)
        {
            if (key[i] == key[j])
            {
                printf("Key must not contain duplicate characters!\n");
                exit(1);
            }
        }
    }
    return true;
}

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    string key = argv[1];
    if (key_is_valid(key))
    {
        string plaintext = get_string("plaintext: ");
        const char *ciphertext = encrypt(plaintext, key);
        printf("ciphertext: %s\n", ciphertext);
        return 0;
    }
}