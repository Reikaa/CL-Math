__kernel void mmult_square_float(__global float* A, __global float* B, __global float* C, int size)
{

	int l = get_global_id(0);

	int x = l / size;
	int y = l % size;

	float val = 0;
	for(int i = 0; i < size; i++)
	{
		val += A[y + size * i] * B[i + size * x];
	}

	C[x * size + y] = val;

}
