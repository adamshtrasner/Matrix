#include <iostream>

#ifndef EX5_MATRIX_H
#define EX5_MATRIX_H

class Matrix {

private:

    float **mat_ = nullptr;
    int rows_;
    int cols_;

    /**
     * The function deletes/frees the memory of the the 2-dimensional array - mat_
     */
    void FreeMatrix() noexcept;

public:

    /**
     * Constructs matrix rows * cols (need to make sure rows, cols are non negative).
     * Initiates all elements to 0.
     * @param rows number of rows
     * @param cols number of columns
     */
    Matrix(int rows, int cols) noexcept(false);

    /**
     * Constructs 1*1 matrix, where the single element is initiated to 0.
     */
    Matrix() noexcept(false);

    /**
     * Constructs matrix from another matrix.
     * @param m type Matrix&
     */
    Matrix(const Matrix &m) noexcept(false);

    /**
     * @return the amount of rows (int).
     */
    int GetRows() const noexcept;

    /**
     * @return the amount of columns (int).
     */
    int GetCols() const noexcept;

    /**
     * Transforms a matrix into a column vector.
     * @return the Matrix with the new mat_
     */
    Matrix Vectorize() noexcept(false);

    /**
     * Prints matrix elements, no return value (void).
     * Prints space after each element (not including the last element in the row).
     * Prints new line after each row (not including the last row).
     */
    void Print() const noexcept;

    /**
     *
     * @param rhs (Matrix &)
     * @return Matrix& after the assignment
     */
    Matrix& operator=(const Matrix &rhs) noexcept(false);

    /**
     * Parenthesis indexing - const
     * @param i an integer
     * @param j an integer
     * @return the float number which lies in mat_[i][j]
     */
    float operator()(int i, int j) const noexcept(false);

    /**
     * Parenthesis indexing - non const
     * @param i an integer
     * @param j an integer
     * @return the float number which lies in mat_[i][j]
     */
    float& operator()(int i, int j) noexcept(false);

    /**
     * Brackets indexing with one index - const
     * @param k
     * @return the float number which lies in mat_[i][j]
     * when (r = row length), and (k = i*r + j).
     */
    float operator[](int k) const noexcept(false);

    /**
     * Brackets indexing with one index - non const
     * @param k
     * @return the float number which lies in mat_[i][j]
     * when (r = row length), and (k = i*r + j).
     */
    float& operator[](int k) noexcept(false);

    /**
     *
     * @param rhs (Matrix &)
     * @return a new Matrix which is the result of the multiplication
     * between this and rhs
     */
    Matrix operator*(const Matrix &rhs) const noexcept(false);

    /**
     * Multiplication of matrix with scalar from the right
     * @param m (Matrix &)
     * @param s (float) a scalar
     * @return the result of the multiplication of this * s
     */
    Matrix operator*(float s) const noexcept(false);

    /**
     * Multiplication of matrix with scalar from the left
     * @param s (float) a scalar
     * @param rhs (Matrix &)
     * @return the result of the multiplication of s * rhs
     */
    friend Matrix operator*(float s, const Matrix &rhs) noexcept(false);

    /**
     *
     * @param rhs (Matrix &)
     * @return The result of multiplication of this matrix by the matrix rhs
     */
    Matrix& operator*=(const Matrix& rhs) noexcept(false);

    /**
     *
     * @param s (float) a scalar
     * @return The result of multiplication of this matrix by the scalar s
     */
    Matrix& operator*=(float s) noexcept;

    /**
     * Scalar division on the right.
     * @param s (float)
     * @return a new matrix which is this/s.
     */
    Matrix operator/(float s) const noexcept(false);

    /**
     * Scalar division of this matrix.
     * @param s (float)
     * @return a new matrix which is this/ s;
     */
    Matrix& operator/=(float s) noexcept(false);

    /**
     * Matrix addition.
     * @param rhs (Matrix &)
     * @return The addition of the 2 matrices m1 and m2
     */
    Matrix operator+(const Matrix& rhs) const noexcept(false);

    /**
     * Matrix addition accumulation
     * @param rhs (Matrix &)
     * @return The addition of the given matrix rhs to the matrix of this.
     */
    Matrix& operator+=(const Matrix& rhs) noexcept(false);

    /**
     * Matrix scalar addition
     * @param s (float)
     * @return The addition of each element in this matrix by a scalar s.
     */
    Matrix& operator+=(float s) noexcept;

    /**
     * Checks for Equality between 2 matrices
     * @param rhs (Matrix &)
     * @return true if the matrices are equal, false otherwise
     */
    bool operator==(const Matrix& rhs) const noexcept;

    /**
     * Checks for Inequality between 2 matrices
     * @param rhs (Matrix &)
     * @return true if the matrices are NOT equal, false otherwise
     */
    bool operator!=(const Matrix& rhs) const noexcept;

    /**
     * Output stream
     * @param output (std::ostream &)
     * @param rhs (Matrix &)
     * @return prints the given matrix and return an output stream
     */
    friend std::ostream& operator<<(std::ostream &output, const Matrix &rhs) noexcept;

    /**
     * Input stream
     * @param input (std::istream &)
     * @param rhs (Matrix &)
     * @return The program the elements of the matrix as an input from the user,
     * and return the input stream.
     */
    friend std::istream &operator>>(std::istream &input, Matrix& rhs) noexcept(false);

    /**
     * Destroys the matrix.
     */
    ~Matrix() noexcept;
};

#endif //EX5_MATRIX_H
