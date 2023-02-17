#include "helpers.h"
#include <stdlib.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float red = image[i][j].rgbtRed;
            float green = image[i][j].rgbtGreen;
            float blue = image[i][j].rgbtBlue;
            int grey = round((red + blue + green) / 3);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = grey;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int halfway = width / 2;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < halfway; j++)
        {
            // Save RGB values of left pixels to variable
            int red_left = image[i][j].rgbtRed;
            int green_left = image[i][j].rgbtGreen;
            int blue_left = image[i][j].rgbtBlue;

            // Assign RGB values of left pixels as RGB values of the mirrored right pixel
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;

            // Assign RGB values of right pixels as RGB values of the mirrored left pixel
            image[i][width - 1 - j].rgbtRed = red_left;
            image[i][width - 1 - j].rgbtGreen = green_left;
            image[i][width - 1 - j].rgbtBlue = blue_left;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize array showing how much the [height, width] coordinates must be shifted to get the coords of a 3x3 grid surrounding pixel [0, 0]
    int n_coords = 9;
    int coords[n_coords][2];

    int x = 0;
    int y = 0;

    for (int i = -1; i < 2; i ++)
    {
        for (int j = -1; j < 2; j ++)
        {
            coords[x][y] = i;
            coords[x][y + 1] = j;
            x++;
        }
    }

    RGBTRIPLE image_edges[height][width]; // Make 2D array to store blurred RGB values of pixels

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sums[] = {0.0, 0.0, 0.0}; // Sum the RGB values of the pixels in the 3x3 grid. index 0,1,2 = red, blue, green.
            int count = 0; // Count number of valid pixels

            // Find the average of the RGB values of all pixels in a 3x3 grid centred on the current pixel
            for (int k = 0; k < n_coords; k++)
            {
                // Adjusted coords to get each pixel in 3x3 grid
                int h = i + coords[k][0];
                int w = j + coords[k][1];

                if (h >= 0 && h < height && w >= 0 && w < width) // If pixel is within the bitmap coords, add its RGB values to the sum
                {
                    sums[0] += image[h][w].rgbtRed;
                    sums[1] += image[h][w].rgbtGreen;
                    sums[2] += image[h][w].rgbtBlue;
                    count++;
                }
            }
            // add the average RGB values to our holder array
            image_edges[i][j].rgbtRed = round(sums[0] / count);
            image_edges[i][j].rgbtGreen = round(sums[1] / count);
            image_edges[i][j].rgbtBlue = round(sums[2] / count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_edges[i][j]; // assign the blurred pixel values to the original bit map
        }
    }
    return;
}

// Rnsure rgb values are capped at 255
int cap(int rgb_value)
{
    if (rgb_value > 255)
    {
        return 255;
    }
    return rgb_value;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize arrays showing how much the [height, width] coordinates must be shifted to get the Gx and G6 coords, as well as the multipliers for each coord

    int n_coords = 6;
    int Gx_coords[6][2] = {{-1, -1}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 1}};
    int Gx_multiplier[6] = {-1, 1, -2, 2, -1, 1};
    int Gy_coords[6][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {1, -1}, {1, 0}, {1, 1}};
    int Gy_multiplier[6] = {-1, -2, -1, 1, 2, 1};

    RGBTRIPLE image_edges[height][width]; // Make 2D array to store blurred RGB values of pixels

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // These arrays will store Gx and Gy values for the R, G, and B pixels
            int Gx[] = {0, 0, 0};
            int Gy[] = {0, 0, 0};

            for (int k = 0; k < n_coords; k++)
            {
                // Adjusted coords to get each pixel forming an edge in the x dimension
                int Gx_h = i + Gx_coords[k][0];
                int Gx_w = j + Gx_coords[k][1];

                // If pixel is within the bitmap coords, add its multiplied RGB values to the sum
                if (Gx_h >= 0 && Gx_h < height && Gx_w >= 0 && Gx_w < width)
                {

                    Gx[0] += image[Gx_h][Gx_w].rgbtRed * Gx_multiplier[k];
                    Gx[1] += image[Gx_h][Gx_w].rgbtGreen * Gx_multiplier[k];
                    Gx[2] += image[Gx_h][Gx_w].rgbtBlue * Gx_multiplier[k];
                }

                // Adjusted coords to get each pixel forming an edge in the y dimension
                int Gy_h = i + Gy_coords[k][0];
                int Gy_w = j + Gy_coords[k][1];

                if (Gy_h >= 0 && Gy_h < height && Gy_w >= 0 && Gy_w < width)
                {
                    Gy[0] += image[Gy_h][Gy_w].rgbtRed * Gy_multiplier[k];
                    Gy[1] += image[Gy_h][Gy_w].rgbtGreen * Gy_multiplier[k];
                    Gy[2] += image[Gy_h][Gy_w].rgbtBlue * Gy_multiplier[k];
                }
            }

            // Apply Sobel filter algorithm to each color
            double red = sqrt(pow(Gx[0], 2) + pow(Gy[0], 2));
            double green = sqrt(pow(Gx[1], 2) + pow(Gy[1], 2));
            double blue = sqrt(pow(Gx[2], 2) + pow(Gy[2], 2));

            // Output of sobel algorithm for each color is rounded to an integer, capped at 255 maximum, and assigned to bitmap
            image_edges[i][j].rgbtRed = cap(round(red));
            image_edges[i][j].rgbtGreen = cap(round(green));
            image_edges[i][j].rgbtBlue = cap(round(blue));
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = image_edges[i][j]; // Assign the edge pixel values to the original bit map
        }
    }
    return;
}