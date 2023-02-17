#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through all pixels in image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //if pixel is black, make it baby blue
            if (image[i][j].rgbtBlue == 0x00 && image[i][j].rgbtBlue == 0x00 && image[i][j].rgbtBlue == 0x00)
            {
                image[i][j].rgbtBlue = 0xe3;
                image[i][j].rgbtGreen = 0xd8;
                image[i][j].rgbtRed = 0x9a;
            }

        }
    }
}
