#ifndef SOL_FILTERS_H
#define SOL_FILTERS_H

#include "Matrix.h"


Matrix Quantization(const Matrix& image,int levels);

Matrix Blur(const Matrix& image);

Matrix Sobel(const Matrix& image);


#endif //SOL_FILTERS_H
