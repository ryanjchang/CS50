#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //we already do the for loop in the main function
    //we also already do the image stuff
    //how to convert a byte into an int?
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            int average = (int)((red + green + blue) / 3);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width / 2; j++){
            //store left side
            RGBTRIPLE temp = image[i][j];
            //make left side right
            image[i][j] = image[i][width - j - 1];
            //make right side left
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //sum all the values of pixels adjacent to current pixel
    //average the values out

    //to avoid triple nested loop
    //maybe do a calculate as we go and then set at the very end?
    RGBTRIPLE blurred[height][width];

    //apparently we can do a horizontal blur and then a vertical blur afterward to get O(h*w)
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            int count = 0;
            //maybe do a 3x3 grid, and if that tile exists, add it to the total sum
            for(int a = -1; a < 2; a++){
                for(int b = -1; b < 2; b++){
                    if(i + a >= 0 && i + a < height &&
                        j + b >= 0 && j + b < width){
                        //if valid pixel, get its value
                        sumRed += image[i + a][j + b].rgbtRed;
                        sumGreen += image[i + a][j + b].rgbtGreen;
                        sumBlue += image[i + a][j + b].rgbtBlue;
                        count += 1;
                    }
                }
            }
            //store blurred pixel as average of all adjacent pixels
            blurred[i][j].rgbtRed = (int)(sumRed / count);
            blurred[i][j].rgbtGreen = (int)(sumGreen / count);
            blurred[i][j].rgbtBlue = (int)(sumBlue / count);
        }
    }
    //update with new values at the end to prevent modifying in place
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            image[i][j] = blurred[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edges[height][width];
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            //find a 3x3 grid around it
            //for each of the 3 colors find Gx and Gy
            //take the sum of the 9 pixels and average them out
            //then combine Gx and Gy with sqrt(Gx^2 + Gy^2)

            //find the Gx
            int sumRedx = 0;
            int sumGreenx = 0;
            int sumBluex = 0;
            //find the Gy
            int sumRedy = 0;
            int sumGreeny = 0;
            int sumBluey = 0;
            for(int a = -1; a < 2; a++){
                for(int b = -1; b < 2; b++){
                    if(i + a >= 0 && i + a < height &&
                        j + b >= 0 && j + b < width){
                        //if valid pixel, get its value
                        //use weighted values
                        sumRedx += image[i + a][j + b].rgbtRed * Gx[a + 1][b + 1];
                        sumGreenx += image[i + a][j + b].rgbtGreen * Gx[a + 1][b + 1];
                        sumBluex += image[i + a][j + b].rgbtBlue * Gx[a + 1][b + 1];

                        sumRedy += image[i + a][j + b].rgbtRed * Gy[a + 1][b + 1];
                        sumGreeny += image[i + a][j + b].rgbtGreen * Gy[a + 1][b + 1];
                        sumBluey += image[i + a][j + b].rgbtBlue * Gy[a + 1][b + 1];
                    }
                }
            }
            //store pixels
            int red = round(sqrt(sumRedx * sumRedx + sumRedy * sumRedy));
            int green = round(sqrt(sumGreenx * sumGreenx + sumGreeny * sumGreeny));
            int blue = round(sqrt(sumBluex * sumBluex + sumBluey * sumBluey));

            edges[i][j].rgbtRed = (red > 255) ? 255 : red;
            edges[i][j].rgbtGreen = (green > 255) ? 255 : green;
            edges[i][j].rgbtBlue = (blue > 255) ? 255 : blue;
        }
    }
    //combine them
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            image[i][j] = edges[i][j];
        }
    }
    return;
}
