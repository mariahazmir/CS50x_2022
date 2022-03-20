#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Go through each row
    for (int i = 0; i < height; i++)
    {
        // Go through each pixel
        for (int j = 0; j < width; j++)
        {
            int r = image[i][j].rgbtRed;
            int g = image[i][j].rgbtGreen;
            int b = image[i][j].rgbtBlue;
            // Find the average value of the RGB value
            int avg = round(((float)r + (float)g + (float)b) / 3);
            // Change current RGB value to the average
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Go through each row
    for (int i = 0; i < height; i++)
    {
        // Go through each pixel
        for (int j = 0; j < width; j++)
        {
            int r = image[i][j].rgbtRed;
            int g = image[i][j].rgbtGreen;
            int b = image[i][j].rgbtBlue;
            // Calculate red value
            int sepiaRed = round((0.393 * (float)r) + (0.769 * (float)g) + (0.189 * (float)b));
            // Cap the value at 255
            if (sepiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = sepiaRed;
            }
            // Calculate green value
            int sepiaGreen = round((0.349 * (float)r) + (0.686 * (float)g) + (0.168 * (float)b));
            // Cap the value at 255
            if (sepiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }
            // Calculate blue value
            int sepiaBlue = round((0.272 * (float)r) + (0.534 * (float)g) + (0.131 * (float)b));
            // Cap the value at 255
            if (sepiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Go through each row
    for (int i = 0; i < height; i++)
    {
        // If the horizontal pixel count is an even number
        if (width % 2 == 0)
        {
            for (int j = 0; j < width / 2; j++)
            {
                // Switch pixel positions
                RGBTRIPLE temp = image[i][j];
                image[i][j] = image[i][width - (j + 1)];
                image[i][width - (j + 1)] = temp;
            }
        }
        // If the horizontal pixel count is an odd number
        else if (width % 2 != 0)
        {
            for (int j = 0; j < (width - 1) / 2; j++)
            {
                // Switch pixel positions
                RGBTRIPLE temp = image[i][j];
                image[i][j] = image[i][width - (j + 1)];
                image[i][width - (j + 1)] = temp;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary image
    RGBTRIPLE temp[height][width];
    // Go through each row
    for (int i = 0; i < height; i++)
    {
        // Go through each pixel
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            float counter = 0.00;
            // Get the neighbouring pixels
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int currentX = i + x;
                    int currentY = j + y;

                    // Check for valid neighbouring pexels
                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1))
                    {
                        continue;
                    }

                    // Get the image value
                    sumRed += image[currentX][currentY].rgbtRed;
                    sumGreen += image[currentX][currentY].rgbtGreen;
                    sumBlue += image[currentX][currentY].rgbtBlue;

                    counter++;
                }

                // Calculate the average of neigbhouring pexels
                temp[i][j].rgbtRed = round(sumRed / counter);
                temp[i][j].rgbtGreen = round(sumGreen / counter);
                temp[i][j].rgbtBlue = round(sumBlue / counter);
            }
        }

    }

    // Copy the blurred image to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}