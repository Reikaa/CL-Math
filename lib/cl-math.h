#define DEVICE_TYPE_GPU 1
#define DEVICE_TYPE_CPU 0
#define INIT_ZEROES 0
#define INIT_RANDOM 1

typedef float** Matrix;

class CLMath
{
public:
	void CLSquareMatrixMultiply(float** src_a_matrix, float** src_b_matrix, float** &dest_h, int side_length, int device_type);
	void TestMatrixVectorTransformations(float** in, float** &out, int side_length);
	Matrix CreateSquareMatrix(int side_length, int type);
	void PrintMatrix(Matrix m, int side_length);
};
