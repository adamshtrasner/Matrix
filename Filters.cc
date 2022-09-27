#include <iostream>
#include <cmath>
#include "Filters.h"

#define NUM_SHADES 256

// -------- Static (helper) functions --------
/**
 * The program updates every cell in the matrix to zero if it's
 * less then zero, and 255 if it's greater then 255.
 * @param image a matrix
 */
static void Update(Matrix& image){
    for (int i = 0; i < image.GetRows() * image.GetCols(); i ++){
        if (image[i] < 0){
            image[i] = 0;
        }
        else{
            if (image[i] >= NUM_SHADES){
                image[i] = NUM_SHADES - 1;
            }
        }
    }
}

/**
 *
 * @param a a number (float)
 * @param b a number (float)
 * @return a float number which is the average of a and b - floored
 */
static float AverageFloor(float a, float b){
    return std::floor((a + b) / 2);
}

/**
 *
 * @param m a matrix to operate a convolution on
 * @param conv a 3 x 3 convolution matrix
 * @return a new matrix which is the result of the convolution of the given matrices.
 */
static Matrix Convolution(const Matrix& image, const Matrix& conv){
    // Construct a new matrix
    auto res = Matrix(image.GetRows(), image.GetCols());

    for (int i = 0; i < image.GetRows(); i++){
        for (int j = 0; j < image.GetCols(); j ++){
            if (i == 0) {  // first row of the image matrix
                if (j == 0){  // first column of the image matrix
                    res(i, j) = std::rintf(image(i, j) * conv[4]
                                   + image(i, j + 1) * conv[5]
                                   + image(i + 1, j) * conv[7]
                                   + image(i + 1, j + 1) * conv[8]);
                }
                else{
                    if (j == image.GetCols() - 1){  // last column of the image matrix
                        res(i, j) = std::rintf(image(i, j - 1) * conv[3]
                                       + image(i, j) * conv[4]
                                       + image(i + 1, j - 1) * conv[6]
                                       + image(i + 1, j) * conv[7]);
                    }
                    else{  // any column other then the first and the last one
                        res(i, j) = std::rintf(image(i, j - 1) * conv[3]
                                       + image(i, j) * conv[4]
                                       + image(i, j + 1) * conv[5]
                                       + image(i + 1, j - 1) * conv[6]
                                       + image(i + 1, j) * conv[7]
                                       + image(i + 1, j + 1) * conv[8]);
                    }
                }

            }
            else{
                if (j == 0){  // first column of the image matrix
                    if (i == image.GetRows() - 1){  // last row of the image matrix
                        res(i, j) = std::rintf(image(i - 1, j) * conv[1]
                                       + image(i - 1, j + 1) * conv[2]
                                       + image(i, j) * conv[4]
                                       + image(i, j + 1) * conv[5]);
                    }
                    else{  // any row other then the first and the last one
                        res(i, j) = std::rintf(image(i - 1, j) * conv[1]
                                       + image(i - 1, j + 1) * conv[2]
                                       + image(i, j) * conv[4]
                                       + image(i, j + 1) * conv[5]
                                       + image(i + 1, j) * conv[7]
                                       + image(i + 1, j + 1) * conv[8]);
                    }
                }
                else{
                    if (i == image.GetRows() - 1){  // last row of the image matrix
                        if (j == image.GetCols() - 1){  // last column of the image matrix
                            res(i, j) = std::rintf(image(i - 1, j - 1) * conv[0]
                                           + image(i - 1, j) * conv[1]
                                           + image(i, j - 1) * conv[3]
                                           + image(i, j) * conv[4]);
                        }
                        else{  // // any column other then the first and the last one
                            res(i, j) = std::rintf(image(i - 1, j - 1) * conv[0]
                                           + image(i - 1, j) * conv[1]
                                           + image(i - 1, j + 1) * conv[2]
                                           + image(i, j - 1) * conv[3]
                                           + image(i, j) * conv[4]
                                           + image(i, j + 1) * conv[5]);
                        }
                    }
                    else{
                        if (j == image.GetCols() - 1){  // last column of the image matrix
                            // already dealt with first row and last row,
                            // only left to deal with any row other then last and first
                            res(i, j) = std::rintf(image(i - 1, j - 1) * conv[0]
                                           + image(i - 1, j) * conv[1]
                                           + image(i, j - 1) * conv[3]
                                           + image(i, j) * conv[4]
                                           + image(i + 1, j - 1) * conv[6]
                                           + image(i + 1, j) * conv[7]);
                        }
                        else{  // we're not in the edges of the image matrix
                            res(i, j) = std::rintf(image(i - 1, j - 1) * conv[0]
                                           + image(i - 1, j) * conv[1]
                                           + image(i - 1, j + 1) * conv[2]
                                           + image(i, j - 1) * conv[3]
                                           + image(i, j) * conv[4]
                                           + image(i, j + 1) * conv[5]
                                           + image(i + 1, j - 1) * conv[6]
                                           + image(i + 1, j) * conv[7]
                                           + image(i + 1, j + 1) * conv[8]);
                        }
                    }
                }
            }
        }
    }
    return res;
}

// -------- End of static functions --------

// -------- Start of the filters functions --------

/**
 *
 * @param image a matrix
 * @param levels an integer
 * @return a new matrix which is the result of quantization on the
 * original matrix.
 */
Matrix Quantization(const Matrix& image,int levels) {
    // Number of colours
    int num_colours = NUM_SHADES / levels;

    // Construct the new matrix
    Matrix quant(image.GetRows(), image.GetCols());

    if (levels == 1){
        float colour = AverageFloor(NUM_SHADES - 1, 0);
        for (int i = 0; i < image.GetRows() * image.GetCols(); i ++){
            quant[i] = colour;
        }
    }

    else{
        // Constructing arrays which represent the bounds of the colours and their average
        auto *colours = new float[levels];
        auto *upper = new float[levels];
        auto *lower = new float[levels];

        lower[0] = 0;
        upper[0] = (float)num_colours - 1;
        colours[0] = AverageFloor(lower[0], upper[0]);

        for (int i = 1; i < levels; i ++){
            lower[i] = lower[i-1] + (float)num_colours;
            upper[i] = upper[i-1] + (float)num_colours;
            colours[i] = AverageFloor(lower[i], upper[i]);
        }

        for (int i = 0; i < image.GetRows() * image.GetCols(); i ++){
            for (int j = 0; j < levels; j ++){
                if (image[i] < upper[j]){
                    quant[i] = colours[j];
                    break;
                }
            }
        }

        // Free the arrays
        delete[] colours;
        delete[] lower;
        delete[] upper;
    }

    // Return the matrix
    return quant;
}

/**
 *
 * @param image a matrix
 * @return a new matrix which is the result of blurring the
 * original matrix.
 */
Matrix Blur(const Matrix& image) {
    Matrix conv(3, 3);

    // Creating the convolution matrix:
    // [1 2 1]
    // [2 4 2] * (1/16)
    // [1 2 1]
    conv[0] = conv[2] = conv[6] = conv[8] = 1;
    conv[4] = 4;
    conv[1] = conv[3] = conv[5] = conv[7] = 2;
    conv *= 0.0625;

    Matrix new_conv = Convolution(image, conv);

    Update(new_conv);

    return new_conv;
}

/**
 *
 * @param image a matrix
 * @return a new matrix which is the result of "sobeling" the
 * original matrix.
 */
Matrix Sobel(const Matrix& image){
    Matrix conv_gx(3, 3);

    // Creating the G_x convolution matrix:
    // [1 0 -1]
    // [2 0 -2] * (1/8)
    // [1 0 -1]
    conv_gx[0] = conv_gx[6] = 1;
    conv_gx[2] = conv_gx[8] = -1;
    conv_gx[3] = 2;
    conv_gx[5] = -2;
    conv_gx *= 0.125;

    Matrix conv_gy(3, 3);

    // Creating the G_y convolution matrix:
    // [1   2  1]
    // [0   0  0] * (1/8)
    // [-1 -2 -1]
    conv_gy[0] = conv_gy[2] = 1;
    conv_gy[1] = 2;
    conv_gy[6] = conv_gy[8] = -1;
    conv_gy[7] = -2;
    conv_gy *= 0.125;

    Matrix sobel = Convolution(image, conv_gx) + Convolution(image, conv_gy);

    // Check that range is between 0 - 255
    Update(sobel);

    return sobel;
}
