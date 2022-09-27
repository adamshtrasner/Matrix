#include <fstream>
#include "Matrix.h"
#include "Filters.h"

#define MAIN

#ifdef MAIN

/**
 * Reads the file (given by file path) to
 * the matrix which is referenced.
 * @param filePath path to some file which includes
 * matrix values (in range [0, 255]).
 * @param mat reference to matrix.
 * @return true if the operation succeeded, false otherwise.
 */
bool readFileToMatrix(const std::string &filePath, Matrix &mat)
{
	std::fstream file;
	file.open(filePath.c_str());
	if (!file.is_open())
	{
		return false;
	}
	file >> mat;
	file.close();
	return true;
}


/**
 * Writes the references matrix to the given file path.
 * @param filePath path to some file where the matrix
 * is going to be written to.
 * @param mat the matrix which to be written to the file.
 * @return true if the operation succeeded, false otherwise.
 */
bool writeMatrixToFile(const std::string &filePath, const Matrix &mat)
{
	std::ofstream file(filePath);
	if (!file.is_open())
	{
		return false;
	}
	file << mat;
	file.close();
	return true;
}


/**
 * Program's main
 * @param argc count of args
 * @param argv args values
 * @return program exit status code
 */
int main(int argc, char **argv)
{
    if (argc < 4){
        exit(1);
    }

	const std::string filePath = (std::string) argv[1];
	const std::string chosenOperator = (std::string) argv[2];
	const std::string outputFilePath = (std::string) argv[3];



	Matrix matrix(128, 128);
	readFileToMatrix(filePath, matrix);

	Matrix result;
	if (chosenOperator == "quant")
	{
		int levels = 8;
		result = Quantization(matrix, levels);
	}
	else if (chosenOperator == "blur")
	{
		result = Blur(matrix);
	}
	else if (chosenOperator == "sobel")
	{
		result = Sobel(matrix);
	}
	else
	{
		std::cerr << "Invalid operator selected." << std::endl;
		exit(1);
	}

	writeMatrixToFile(outputFilePath, result);
    return 0;
}
#endif
