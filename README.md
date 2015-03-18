# About CL-Math
Mathematics library taking advantage of OpenCL hardware acceleration. 
Written for UAA's CSCE 448 course.

# Current Status
Single-precision floating-point square matrix multiplication is all that is currently implemented. 
May be extended to other operations in the future. 

# Usage

"lib/cl-math.h" must be included in any code you wish to use it in.
```c++
#include <lib/cl-math.h>
```

Multiplication of square single-precision floating-point matrices is as simple as the following:
```c++
CLMath* math = new CLMath();

Matrix a = math->CreateSquareMatrix(SIZE, INIT_RANDOM); // or INIT_ZEROES
Matrix b = math->CreateSquareMatrix(SIZE, INIT_RANDOM); // or INIT_ZEROES
Matrix c;

math->CLSquareMatrixMultiply(a, b, c, SIZE, DEVICE_TYPE_GPU); // or DEVICE_TYPE_CPU
```
The matrices can be printed to console as shown:
```c++
math -> PrintMatrix(c, SIZE);
```
Matrix elements can be directly interacted with as floating point values:
```c++
Matrix a = math->CreateSquareMatrix(3, INIT_ZEROES);
a[1][1] = 5;
```
The CreateSquareMatrix function takes a side length as the first argument. Matrices are later accessed using zero-indexing, so a 3x3 matrix generated using CreateSquareMatrix(3, INIT_ZEROES) is valid from [0-2][0-2].
