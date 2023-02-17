#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // keep asking for height until it's between 1 and 8
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    int height_index = height - 1;

    // row
    for (int row = 0; row < height; row++)
    {
        // left column
        for (int l_col = 0; l_col < height; l_col++)
        {
            int first_block = height_index - row;
            if (l_col < first_block)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        }

        // central column
        printf("  ");

        // right column
        for (int r_col = 0; r_col < height; r_col++)
        {
            int last_block = row;
            if (r_col <= row)
            {
                printf("#");
            }
        }
        
        printf("\n");
    }
}