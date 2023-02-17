#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// Create new datatype to store BYTEs of data
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for command line args
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open image
    FILE *image = fopen(argv[1], "r");
    if (image == NULL)
    {
        printf("File does not exist.");
        return 1;
    }

    // Initialize values
    const int BLOCK_SIZE = 512;
    BYTE buffer[BLOCK_SIZE];
    int jpg_n = -1;
    char output_jpg[8];
    FILE *outfile = NULL;

    // Read through file in blocks of 512 BYTEs
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, image) == BLOCK_SIZE)
    {
        // Check if first 4 bytes of block is consistent with first 4 bytes of a jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (outfile != NULL) // If a current jpg is open, close it
            {
                fclose(outfile);
            }

            // Create a new output jpg file
            jpg_n++;
            sprintf(output_jpg, "%.3i.jpg", jpg_n);
            outfile = fopen(output_jpg, "w");
            if (outfile == NULL)
            {
                fclose(image);
                fclose(outfile);
                printf("Could not create %s.\n", output_jpg);
                return 1;
            }
        }
        // Write blocks to jpg output file
        if (outfile != NULL)
        {
            fwrite(buffer, 1, BLOCK_SIZE, outfile);
        }
    }
    fclose(image);
    fclose(outfile);
}