#ifndef arrayUtils_H
#define arrayUtils_H
#include <functional>
#include <iomanip>
#include <iostream>

#include "../../../../../../../Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include/cuda_runtime_api.h"
#include "../../../../../../../Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include/device_launch_parameters.h"
#include "cudaFuncs.h"

namespace NeurelNetwork
{
	template<typename Type>
	class Layer;
	namespace ArrayUtils
	{

template<typename Type>
class Array
{
private:
	unsigned int R;
	unsigned int C;

	bool safetyH;
	bool safetyD;

	Type* H_Ptr;
	Type* D_Ptr;

public:
	Array();

	~Array();

	__host__ Array(int Rows, int Columns);

	__host__ Array(int Rows, int Columns, Type* H_Ptr, Type* D_Ptr);

	__host__ Array(Array<Type>&& other);
	__host__ Array& operator=(Array<Type>&& other);

	__host__ void updateDevice();

	__host__ unsigned int GetColumns() const;

	__host__ unsigned int GetRows() const;

	__host__ void print() const;

	__host__ Type* getHPtr();

	friend class Layer<Type>;

};

template <typename Type>
Array<Type>::Array():
		R(0), C(0), safetyH(true), safetyD(true), H_Ptr(nullptr), D_Ptr(nullptr)
{
}

template <typename Type>
Array<Type>::~Array()
{
	if (safetyH)
		delete[] H_Ptr;
	if (safetyD)
		cudaFree(D_Ptr);
}

template <typename Type>
Array<Type>::Array(int Rows, int Columns):
		R(Rows), C(Columns)
{
	H_Ptr = new Type[R*C]();
	cudaMalloc((void**)&D_Ptr, sizeof(Type)*R*C);
	cudaMemcpy(D_Ptr, H_Ptr, sizeof(Type)*R*C, cudaMemcpyHostToDevice);
}

template <typename Type>
Array<Type>::Array(int Rows, int Columns, Type* H_Ptr, Type* D_Ptr):
		R(Rows), C(Columns)
{
	this->H_Ptr = H_Ptr;
	this->D_Ptr = D_Ptr;
}

template <typename Type>
Array<Type>::Array(Array<Type>&& other)
{
	R = other.R;
	C = other.C;
	H_Ptr = other.H_Ptr;
	D_Ptr = other.D_Ptr;
	other.R = 0;
	other.C = 0;
	other.H_Ptr = nullptr;
	other.D_Ptr = nullptr;
}

template <typename Type>
Array<Type>& Array<Type>::operator=(Array<Type>&& other)
{
	if (safetyH)
		delete[] H_Ptr;
	if (safetyD)
		cudaFree(D_Ptr);
	R = other.R;
	C = other.C;
	H_Ptr = other.H_Ptr;
	D_Ptr = other.D_Ptr;
	other.R = 0;
	other.C = 0;
	other.H_Ptr = nullptr;
	other.D_Ptr = nullptr;
	return *this;
}

template <typename Type>
void Array<Type>::updateDevice()
{
	cudaMemcpy(D_Ptr, H_Ptr, sizeof(Type)*R*C, cudaMemcpyHostToDevice);
}

/*template <typename Type>
Array<Type> Array<Type>::AddArr(Array<Type>& arr)
{
	if (arr.R != R)
	{
		std::cerr << "Array Rows Mismatch" << std::endl;
		return 0;
	}
	if (arr.C != C)
	{
		std::cerr << "Array Columns Mismatch" << std::endl;
		return 0;
	}

	const int ArraySize = R*C;

	const int blocks = (ArraySize + 1023)/ 1024; //this calculation is to force a round up bc casting to int is basically floor(val) so in this case we are adding a value close to 1 but barely below

	Type* O_Ptr;
	cudaMalloc(O_Ptr, sizeof(Type)*ArraySize);

	GpuAdd <<<blocks, 1024>>> (D_Ptr, arr.D_Ptr, O_Ptr, ArraySize);

	return Array<Type>(R, C, nullptr, D_Ptr);

}*/

template <typename Type>
unsigned int Array<Type>::GetColumns() const
{
	return C;
}

template <typename Type>
unsigned int Array<Type>::GetRows() const
{
	return R;
}

template <typename Type>
void Array<Type>::print() const
{

	if (H_Ptr != nullptr)
	{
		std::cout << "Array Host Vals" << std::endl;
		for (int index = 0; index < R * C; index++)
		{
			if (index % C == 0)
			{
				std::cout << std::endl;
			} else
			{
				std::cout << ' ';
			}
			std::cout << H_Ptr[index];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	Type* tempDPtr = new Type[R*C];
	cudaMemcpy(tempDPtr, D_Ptr, sizeof(Type)*R*C, cudaMemcpyDeviceToHost);
	std::cout << "Array device Vals: ";
	for (int index = 0; index < R * C; index++)
	{
		if (index % C == 0)
		{
			std::cout << std::endl;
		} else
		{
			std::cout << ' ';
		}
		std::cout << tempDPtr[index];
	}
	std::cout << std::endl;
}

template <typename Type>
Type* Array<Type>::getHPtr()
{
	return H_Ptr;
}


/*template <typename Type>
void Array<Type>::GpuAdd(Type* SelfD_Ptr, Type* D_Ptr, Type* O_Ptr, const int size)
{
	const int threadIndx = threadIdx.x + blockIdx.x * blockDim.x;
	if (threadIndx < size)
	{
		O_Ptr[threadIndx] = SelfD_Ptr[threadIndx] + D_Ptr[threadIndx];
	}
}*/


	}
}
#endif //end of arrayUtils_H




