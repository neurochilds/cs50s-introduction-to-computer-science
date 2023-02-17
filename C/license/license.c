#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check for command line args
    if (argc != 2)
    {
        printf("Usage: ./read infile\n");
        return 1;
    }

    int PLATE_SIZE = 7; // 6 chars + \0 to end string

    // Create buffer to read into
    char buffer[PLATE_SIZE];

    // Create array to store plate numbers
    char *plates[8];

    FILE *infile = fopen(argv[1], "r");

    int idx = 0;

    while (fread(buffer, 1, PLATE_SIZE, infile) == PLATE_SIZE)
    {
        // Replace '\n' with '\0'
        buffer[6] = '\0';

        // To copy the plate number from the buffer into the plates array, first allocate PLATE_SIZE memory to the current element of plates array
        plates[idx] = malloc(PLATE_SIZE);

        // Then, copy each character from the buffer into each character of the plates element
        for (int i = 0; i < PLATE_SIZE; i++)
        {
            plates[idx][i] = buffer[i];
        }

        idx++;

    }

    // Print out plates
    for (int i = 0; i < 8; i++)
    {
        printf("%s\n", plates[i]);
    }

    // Free up memory that was assigned to plates
    for (int i = 0; i < idx; i++)
    {
        free(plates[i]);
    }

    fclose(infile);
}
