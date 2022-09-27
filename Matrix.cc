#include <iostream>
#include "Matrix.h"
#include "MatrixException.h"

#define DIMENSION_ERROR "Invalid matrix dimensions.\n"
#define DIVISION_BY_ZERO_ERROR "Division by zero.\n"
#define INDEX_RANGE_ERROR "Index out of range.\n"
#define STREAM_ERROR "Error loading from input stream.\n"
#define BAD_ALLOC "Allocation failed.\n"

using namespace std;


// -------- Static (helper) functions --------

static float **AllocateMatrix(int rows, int cols) {
    try{
        // Allocate memory for the matrix
        auto **mat = (float **)new float*[rows]; // Rows of the matrix
        // Allocate memory for each pointer - rows of the matrix
        for (int i = 0; i < rows; i++){
            mat[i] = (float *)new float[cols];
        }
        return mat;

    } catch (const std::bad_alloc& e) {
        throw MatrixException(BAD_ALLOC);
    }

}

// -------- End of static functions --------

// -------- Private functions --------

void Matrix::FreeMatrix() noexcept {
    if (this->mat_){
        for (int i = 0; i < rows_; i++){
            delete[] mat_[i];
        }
        delete[] mat_;
        mat_ = nullptr;
    }
}

// -------- End of private functions --------

// The implementation of the matrix's public functions:

Matrix::Matrix(int rows, int cols) noexcept(false) {
    if (rows <= 0 or cols <= 0){
        throw MatrixException(DIMENSION_ERROR);
    }

    this->rows_ = rows;
    this->cols_ = cols;

    this->mat_ = AllocateMatrix(this->rows_, this->cols_);

    // Initialize all elements to zero
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j ++){
            mat_[i][j] = 0;
        }
    }
}

Matrix::Matrix() noexcept(false) : Matrix::Matrix(1, 1) {}

Matrix::Matrix (const Matrix &m) noexcept(false) {
    this->rows_ = m.rows_;
    this->cols_ = m.cols_;

    // Allocate memory for the matrix
    this->mat_ = AllocateMatrix(this->rows_, this->cols_);

    // Initialize all elements of this mat to be equal to elements of m
    for (int i = 0; i < rows_; i++){
        for (int j = 0; j < cols_; j ++){
            mat_[i][j] = m.mat_[i][j];
        }
    }
}

int Matrix::GetRows() const noexcept{
    return this->rows_;
}

int Matrix::GetCols() const noexcept{
    return this->cols_;
}

Matrix Matrix::Vectorize() noexcept(false){
    if (this->cols_ == 1){
        // if the matrix is already a 1 column matrix
        return *this;
    }
    // construct a new matrix with 1 column and (rows_ + cols_) rows
    int new_rows = rows_ * cols_;
    auto **new_mat = AllocateMatrix(new_rows, 1);

    int k = 0;
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j++){
            new_mat[k][0] = this->mat_[i][j];
            k ++;
        }
    }

    // Free the current matrix
    FreeMatrix();

    // Modify the fields and return the matrix
    this->mat_ = new_mat;
    this->rows_ = new_rows;
    this->cols_ = 1;

    return *this;
}

void Matrix::Print() const noexcept{
    for (int i = 0; i < rows_; i++){
        for (int j = 0; j < cols_; j ++) {
            cout << mat_[i][j] << " ";
        }
        if (i != rows_ - 1){
            cout << endl;
        }
    }
}

Matrix &Matrix::operator=(const Matrix &m) noexcept(false) {
    if (&m == this){
        return *this;
    }

    // Frees matrix if necessary
    FreeMatrix();

    this->rows_ = m.GetRows();
    this->cols_ = m.GetCols();

    try{
        float **new_mat = AllocateMatrix(this->rows_, this->cols_);
        // The assignment
        for (int i = 0; i < rows_; i++){
            for (int j = 0; j < cols_; j ++){
                new_mat[i][j] = m.mat_[i][j];
            }
        }
        this->mat_ = new_mat;

        return *this;

    } catch (const std::bad_alloc& e) {
        throw MatrixException(BAD_ALLOC);
    }
}

float Matrix::operator()(int i, int j) const noexcept(false) {
    if ((i < 0) || (i >= this->rows_) || (j < 0) || (j >= this->cols_)){
        throw MatrixException(INDEX_RANGE_ERROR);
    }
    return this->mat_[i][j];
}

float &Matrix::operator()(int i, int j) noexcept(false) {
    if ((i < 0) || (i >= this->rows_) || (j < 0) || (j >= this->cols_)){
        throw MatrixException(INDEX_RANGE_ERROR);
    }
    return this->mat_[i][j];
}

float Matrix::operator[](int k) const noexcept(false) {
    // length of each row in the matrix
    if (k < 0){
        throw MatrixException(INDEX_RANGE_ERROR);
    }
    int r = this->cols_;
    int i = k / r;
    if (i >= this->rows_){
        throw MatrixException(INDEX_RANGE_ERROR);
    }
    int j = k % r;
    return this->mat_[i][j];
}

float &Matrix::operator[](int k) noexcept(false) {
    // length of each row in the matrix
    if (k < 0){
        throw MatrixException(INDEX_RANGE_ERROR);
    }
    int r = this->cols_;
    int i = k / r;
    if (i >= this->rows_){
        throw MatrixException(INDEX_RANGE_ERROR);
    }
    int j = k % r;
    return this->mat_[i][j];
}

Matrix Matrix::operator*(const Matrix &m2) const noexcept(false) {
    // Check if dimensions are valid
    if (this->cols_ != m2.GetRows()){
        throw MatrixException(DIMENSION_ERROR);
    }

    // Dimensions of the matrix - m1 * m2
    int rows = this->rows_;
    int cols = m2.GetCols();

    try{

        Matrix mult(rows, cols);

        // Matrix multiplication algorithm
        for (int i = 0; i < this->rows_; i ++){
            for (int j = 0; j < m2.GetCols(); j ++){
                for (int k = 0; k < this->cols_; k ++){
                    mult.mat_[i][j] += (this->mat_[i][k] * m2.mat_[k][j]);
                }
            }
        }

        return mult;

    } catch (const std::bad_alloc& e) {
        throw MatrixException(BAD_ALLOC);
    }
}

Matrix Matrix::operator*(const float s) const noexcept(false){

    // Dimensions of the matrix - m1
    int rows = this->rows_;
    int cols = this->cols_;

    try{

        Matrix mult(rows, cols);

        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                mult.mat_[i][j] = s * this->mat_[i][j];
            }
        }

        return mult;

    } catch (const std::bad_alloc& e) {
        throw MatrixException(BAD_ALLOC);
    }
}

Matrix operator*(const float s, const Matrix &m) noexcept(false){
    return m*s;
}

Matrix &Matrix::operator*=(const Matrix &m) noexcept(false){
    if (this->cols_ != m.GetRows()){
        throw MatrixException(DIMENSION_ERROR);
    }

    try {
        float **new_mat = AllocateMatrix(this->rows_, m.GetCols());

        for (int i = 0; i < this->rows_; i++){
            for (int j = 0; j < m.GetCols(); j ++){
                new_mat[i][j] = 0;
            }
        }

        for (int i = 0; i < this->rows_; i ++){
            for (int j = 0; j < m.GetCols(); j ++){
                for (int k = 0; k < this->cols_; k ++){
                    new_mat[i][j] += this->mat_[i][k] * m.mat_[k][j];
                }
            }
        }

        FreeMatrix();
        this->mat_ = new_mat;
        this->cols_ = m.GetCols();

        return *this;

    } catch (const std::bad_alloc& e) {
        throw MatrixException(BAD_ALLOC);
    }
}

Matrix &Matrix::operator*=(const float s) noexcept{

    for (int i = 0; i < this->rows_; i ++){
        for (int j = 0; j < this->cols_; j ++){
            this->mat_[i][j] *= s;
        }
    }

    return *this;
}

Matrix Matrix::operator/(float s) const noexcept(false) {
    if (s == 0){
        throw MatrixException(DIVISION_BY_ZERO_ERROR);
    }
    try {

        Matrix div(this->rows_, this->cols_);
        for (int i = 0; i < div.GetRows(); i ++){
            for (int j = 0; j < div.GetCols(); j ++){
                div(i, j) = mat_[i][j]/s;
            }
        }
        return div;

    } catch (const std::bad_alloc& e) {
        throw MatrixException(BAD_ALLOC);
    }
}

Matrix &Matrix::operator/=(float s) noexcept(false) {
    if (s == 0){
        throw MatrixException(DIVISION_BY_ZERO_ERROR);
    }
    for (int i = 0; i < this->rows_; i ++){
        for (int j = 0; j < this->cols_; j ++){
            (*this)(i, j) = (*this)(i, j) / s;
        }
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix &m2) const noexcept(false) {
    // Check for dimensions validity
    if ((this->rows_ != m2.GetRows()) || (this->cols_ != m2.GetCols())){
        throw MatrixException(DIMENSION_ERROR);
    }

    try{

        // Create a new matrix
        Matrix add(this->rows_, this->cols_);

        // Add the 2 matrix and put the result in the new matrix - add
        for (int i = 0; i < this->rows_; i ++){
            for (int j = 0; j < this->cols_; j ++){
                add(i, j) = this->mat_[i][j] + m2(i, j);
            }
        }

        return add;

    } catch (const std::bad_alloc& e) {
        throw MatrixException(BAD_ALLOC);
    }
}

Matrix &Matrix::operator+=(const Matrix &m) noexcept(false) {
    // Check for dimensions validity
    if ((this->rows_ != m.GetRows()) || (this->cols_ != m.GetCols())){
        throw MatrixException(DIMENSION_ERROR);
    }

    // Add the given matrix m to the matrix of this
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            (*this)(i, j) += m(i, j);
        }
    }

    return *this;
}

Matrix &Matrix::operator+=(const float s) noexcept {
    // Add the scalar s to each element in this matrix.
    for (int i = 0; i < rows_; i ++){
        for (int j = 0; j < cols_; j ++){
            (*this)(i, j) += s;
        }
    }
    return *this;
}

bool Matrix::operator==(const Matrix &m2) const noexcept {
    if ((this->rows_ != m2.GetRows()) || (this->cols_ != m2.GetCols())){
        return false;
    }

    for (int i = 0; i < this->rows_; i ++){
        for (int j = 0; j < this->cols_; j++){
            if (this->mat_[i][j] != m2(i, j)){
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &m2) const noexcept {
    return !(this->operator==(m2));
}

ostream &operator<<(ostream &output, const Matrix &m) noexcept {
    for (int i = 0; i < m.GetRows(); i++){
        for (int j = 0; j < m.GetCols(); j ++) {
            output << m(i, j) << " ";
        }
        if (i != m.GetRows() - 1){
            output << endl;
        }
    }
    return output;
}

istream &operator>>(istream &input, Matrix& m) noexcept(false){
    // Check input stream validity.
    if (!input.good()) {
        throw MatrixException(STREAM_ERROR);
    }

    for (int i = 0; i < m.GetRows() * m.GetCols(); i++){
        input >> m[i];
    }

    return input;
}

Matrix::~Matrix() noexcept {
    FreeMatrix();
}