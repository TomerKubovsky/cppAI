//
// Created by ##### on 26/01/2025.
//

#ifndef CUDAFUNCS_H
#define CUDAFUNCS_H

// #include "cuda_runtime_api.h"
#include "../../../../../../../Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.6/include/cuda_runtime_api.h"

namespace NeurelNetwork { namespace CudaFuncs
{
	template <typename Type>
	__global__ void printArr(Type* val, const unsigned int size)
	{
		// printf("the sigma in the hole");
		int indx = threadIdx.x;
		// if (indx < size)
		printf("%f ", val[indx]);
	}

	template <typename Type>
	__global__ void dotProduct
	(Type* inputArr, Type* weightArr, Type* outputArr, const unsigned int neuronSize,
		const unsigned int batchSize, const unsigned int inputSize)
	{
		const unsigned int XIndex = threadIdx.x + blockIdx.x * blockDim.x;
		const unsigned int YIndex = threadIdx.y + blockIdx.y * blockDim.y;
		const unsigned int ZIndex = threadIdx.z + blockIdx.z * blockDim.z;

		/*
		 outputIndex = YIndex + ZIndex * batchSize;
		 inputIndex = XIndex + ZIndex * inputSize;
		 weight index = XIndex + YIndex * neuronSize;
		*/


		if (XIndex < inputSize && YIndex < neuronSize && ZIndex < batchSize)
		{
			atomicAdd((outputArr + YIndex + ZIndex * batchSize),inputArr[XIndex + ZIndex * inputSize] * weightArr[XIndex + YIndex * inputSize]);
			// printf("i am printng a message in the terminal: %f %d %d %d   %f \n", inputArr[XIndex + ZIndex * inputSize] * weightArr[XIndex + YIndex * inputSize], XIndex + ZIndex * inputSize, XIndex + YIndex * inputSize, YIndex + ZIndex * batchSize, outputArr[YIndex + ZIndex * batchSize]);
		}
	}

	template <typename Type>
	__global__ void addArrToRows(Type* biasArr, Type* inputArr, Type* outputArr, const unsigned int inpArrSize)
	{
		const unsigned int indx = threadIdx.x + blockIdx.x * blockDim.x;
		outputArr[indx] = inputArr[indx] + biasArr[];
	}

}}


#endif //CUDAFUNCS_H
