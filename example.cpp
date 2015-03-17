#include <iostream>
#include "lib/cl-math.h"

#define SIZE 3

using namespace std;

int main(void)
{

	CLMath* math = new CLMath();

	Matrix a = math -> CreateSquareMatrix(SIZE, INIT_RANDOM);
	Matrix b = math -> CreateSquareMatrix(SIZE, INIT_RANDOM);
	Matrix c;

	math -> CLSquareMatrixMultiply(a, b, c, SIZE, DEVICE_TYPE_GPU);

	cout << "Matrix A:" << endl;
	math -> PrintMatrix(a, SIZE);

	cout << "Matrix B:" << endl;
	math -> PrintMatrix(b, SIZE);

	cout << endl << "Result Matrix:" << endl;
	math -> PrintMatrix(c, SIZE);

	math -> Free(a, SIZE);
	math -> Free(b, SIZE);
	math -> Free(c, SIZE);

	return 0;

}
