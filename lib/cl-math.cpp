#include <iostream>
#include <CL/cl.h>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include "cl-math.h"


void assertSuccess(cl_int condition);
float** vectorToMatrix(float* vec, int max_x, int max_y);
float* matrixToVector(float **matrix, int max_x, int max_y);

Matrix CLMath::CreateSquareMatrix(int side_length, int type)
{
	Matrix m = new float*[side_length];
	for(int i = 0; i < side_length; i++)
		m[i] = new float[side_length];
	for(int i = 0; i < side_length; i++)
	{
		for(int j = 0; j < side_length; j++)
		{
			if(type == 1)
				m[i][j] = static_cast<float> (rand()) / static_cast <float> (RAND_MAX/100.0);
			else
				m[i][j] = 0;
		}
	}
	return m;
}

void CLMath::PrintMatrix(Matrix m, int side_length)
{
	for(int i = 0; i < side_length; i++)
	{
		for(int j = 0; j < side_length; j++)
		{
			std::cout << m[i][j] << "\t";
		}
		std::cout << std::endl;
	}
}

void CLMath::TestMatrixVectorTransformations(float** in, float** &out, int side_length)
{
	float* temp = matrixToVector(in, side_length, side_length);
	out = vectorToMatrix(temp, side_length, side_length);
}

void CLMath::CLSquareMatrixMultiply(float** src_a_matrix, float** src_b_matrix, float** &dest_h, int side_length, int device_type)
{

	float* src_a_h = matrixToVector(src_a_matrix, side_length, side_length);
	float* src_b_h = matrixToVector(src_b_matrix, side_length, side_length);

	cl_mem src_a_d;
	cl_mem src_b_d;
	cl_mem dest_d;
	cl_mem side_length_d;

	unsigned int mem_size = sizeof(float)*side_length*side_length;

	float* dest_h_vector = (float*)malloc(mem_size);
	cl_uint device_count = 0;
	clGetPlatformIDs(0, 0, &device_count);

	cl_platform_id platform_ids[10];

	clGetPlatformIDs(device_count, platform_ids, NULL);

	cl_int error = 0;
	cl_device_id device_id;
	error = clGetDeviceIDs(platform_ids[0], device_type ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
	assertSuccess(error);

	cl_context context = clCreateContext(0, 1, &device_id, NULL, NULL, &error);
	assertSuccess(error);

	cl_command_queue queue = clCreateCommandQueue(context, device_id, 0, &error);
	assertSuccess(error);

	std::ifstream t("lib/cl-math.cl");
	std::stringstream buffer;
	buffer << t.rdbuf();
	const std::string &tmp = buffer.str();
	const char* kernel_src = tmp.c_str();
	const size_t k_size = tmp.size();

	cl_program program = clCreateProgramWithSource(context, 1, &kernel_src, &k_size, &error);
	assertSuccess(error);

	error = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	assertSuccess(error);

	cl_kernel mmult_kernel = clCreateKernel(program, "mmult_square_float", &error);
	assertSuccess(error);
	src_a_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_a_h, &error);
	assertSuccess(error);
	src_b_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, mem_size, src_b_h, &error);
	assertSuccess(error);
	dest_d = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_size, NULL, &error);
	assertSuccess(error);
	//side_length_d = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int), side_length, &error);
	//assertSuccess(error);
	size_t localWorkSize[2], globalWorkSize[2];

	error = clSetKernelArg(mmult_kernel, 0, sizeof(cl_mem), &src_a_d);
	assertSuccess(error);
	error = clSetKernelArg(mmult_kernel, 1, sizeof(cl_mem), &src_b_d);
	assertSuccess(error);
	error = clSetKernelArg(mmult_kernel, 2, sizeof(cl_mem), &dest_d);
	assertSuccess(error);
	error = clSetKernelArg(mmult_kernel, 3, sizeof(int), &side_length);
	assertSuccess(error);

	localWorkSize[0] = 16;
	localWorkSize[1] = 16;
	globalWorkSize[0] = 1024;
	globalWorkSize[1] = 1024;

	const size_t s = side_length * side_length;

	cl_event completion;
	error = clEnqueueNDRangeKernel(queue, mmult_kernel, 1, NULL, &s, NULL, 0, NULL, &completion);
	assertSuccess(error);

	clWaitForEvents(1, &completion);

	error = clEnqueueReadBuffer(queue, dest_d, CL_TRUE, 0, mem_size, dest_h_vector, 0, NULL, NULL);
	assertSuccess(error);

	dest_h = vectorToMatrix(dest_h_vector, side_length, side_length);

}

float** vectorToMatrix(float* vec, int max_x, int max_y)
{

	float **matrix = new float*[max_y];
	for(int i = 0; i < max_y; i++)
		matrix[i] = new float[max_x];

	for(int i = 0; i < max_x; i++)
		for(int j = 0; j < max_y; j++)
			matrix[i][j] = vec[i + max_x * j];

	return matrix;

}

float* matrixToVector(float **matrix, int max_x, int max_y)
{
	float *vector = new float[max_x * max_y];

	for(int i = 0; i < max_x; i++)
		for(int j = 0; j < max_y; j++)
			vector[i + max_x * j] = matrix[i][j];

	return vector;
}

void assertSuccess(cl_int condition)
{
	if(condition != CL_SUCCESS)
	{
		std::cout << "CL-Math Assertion Failure. Stopping." << std::endl;
		exit(1);
	}
}
