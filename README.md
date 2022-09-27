# Matrix
A C++ implementation of a matrix which represent an image, along with a Filters program which consists of
functions that manipulate images (discussed in the description).
The project's theme is Image Processing.

## Description
The program consists of 2 parts:
* The Matrix class. In this project, the matrix represents a black and white (gray) picture.
There are 256 shades of black and white. Each shade is represented by a natural number in the range 0 to 255 where 255
represents the colour white, and 0 represents the colour black. Therefore, a matrix can represent a picture, 
where each cell is a pixel represented  by a number in that range.
* The Filters program. In this program, I implemented 3 operators from the image processing world:
  1. Quantization - Performs quantization on the input image by the
  given number of levels. Returns new matrix which is
  the result of running the operator on the image.
  2. Gaussian Blurring - Performs gaussian blurring on the input image.
  Returns new matrix which is the result of running the
  operator on the image.
  3. Sobel operator (edge detection) - Performs sobel edge detection on the input image.
  Returns new matrix which is the result of running the
  operator on the image.

## Input and Output
In this project, we'll be using pictures that are represented by a 128x128 matrices.
The inputs are 3 arguments, given in the main program:
1. A file path, representing a picture (the file can be made by the "image2file" program)
2. Name of the filter which we wish to use: "sobel", "blur", or "quant".
3. A file path which will include a printing of the picture's matrix after manipulating it. If you wish to see the picture it self, use the "file2image" program.