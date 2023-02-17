// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Number of buckets in hash table
const unsigned int N = 50000;

// Hash table
node *table[N];
int word_total = 0;
bool loaded = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Make a copy of the word with lowercase characters
    int length = strlen(word);
    char *word_lower = malloc(length + 1);
    if (word_lower != NULL)
    {
        for (int i = 0; i < length; i++)
        {
            word_lower[i] = tolower(word[i]);
        }
        word_lower[length] = '\0';
    }

    unsigned int hash_index = hash(word);

    if (table[hash_index] == NULL) // If nothing exists at this hash_index, return false
    {
        free(word_lower);
        return false;
    }

    node *cursor = table[hash_index]; // Point cursor to node pointed to by this hash_index

    if (strcmp(cursor->word, word_lower) == 0) // If this node contains the word, return true
    {
        free(word_lower);
        return true;
    }

    while (cursor->next != NULL) // Otherwise, if other nodes are linked to this node, check if any of those nodes contain this word
    {
        cursor = cursor->next;
        if (strcmp(cursor->word, word_lower) == 0)
        {
            free(word_lower);
            return true;
        }
    }
    free(word_lower);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    long sum = 0;
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        int num = abs(toupper(word[i]) - 'A') +i; // Convert each character to a number, where A/a = 0, Z/z = 25. '+ i' so that words with the same letters but in different locations return a different hash
        sum += num * num  * num; // Cube each number and add to sum. Cubing the numbers will distribute the hash output over a wider range.
    }
    return sum % N; // Mod N caps the max index at N
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    for (int i = 0; i < N; i++) // Initialize NULL values for array
    {
        table[i] = NULL;
    }

    // Search through dict file and get each word, hash it, add it at its hash index to dict, making linked list of collisions
    char word[LENGTH + 1];
    while (fscanf(dict, "%s", word) != EOF) // Each loop reads the next word in the file and saves it to the 'word' variable
    {
        unsigned int hash_index = hash(word); // Get hash index of word
        node *new = malloc(sizeof(node)); // Allocate memory for new node
        strcpy(new->word, word); // Copy current word to new node
        new->next = table[hash_index]; // Point new node to head of linked list
        table[hash_index] = new; // Set hash_index to point to new node as the new head of the linked list
        word_total++;
    }
    fclose(dict);
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded)
    {
        return word_total;
    }
    return 0;
}

// Unloads dictionary from memory
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            if (table[i]->next != NULL)
            {
                unload_links(table[i]);
            }
            else
            {
                free(table[i]);
            }
        }
    }
    return true;
}

// Unloads all nodes in a linked list of nodes
void unload_links(node *cursor)
{
    if (cursor->next == NULL)
    {
        free(cursor);
    }

    else
    {
        unload_links(cursor->next);
        free(cursor);
    }
}

// // The below code counts the number of words in the dictionary without using a global total_words variable that updates as words are added in the load() function
// unsigned int size(void)
// {
//     unsigned int sum = 0;
//     for (int i = 0; i < N; i++)
//     {
//         if (table[i] != NULL)
//         {
//             if (table[i]->next != NULL)
//             {
//                 sum += count_links(table[i]);
//             }
//             else
//             {
//                 sum++;
//             }
//         }
//     }
//     return sum;
// }

// Counts the number of nodes in a linked list of nodes
// unsigned int count_links(node *cursor)
// {
//     if (cursor->next == NULL)
//     {
//         return 1;
//     }
//     else
//     {
//         return 1 + count_links(cursor->next);
//     }
// }