#ifndef LAYERCLASS_H
#define LAYERCLASS_H

#include <cmath>
#include <complex>
#include <iostream>
#include <random>
#include <chrono>
#include "arrayV2.h"


// using namespace ArrayUtils::ArrayUtils; //bad practice but it's fine for now

namespace NeurelNetwork
{


	template <typename Type>
	class Layer
	{
	private:
		ArrayUtils::Array<Type> Weights;
		ArrayUtils::Array<Type> Biases;
	public:
		Layer(int inputs, int outputs /*outputs aka neurons*/);
		ArrayUtils::Array<Type> Forwards(ArrayUtils::Array<Type>& inputs);

		void setWeights(ArrayUtils::Array<Type>& weights);
		void setBiases(ArrayUtils::Array<Type>& biases);

		const ArrayUtils::Array<Type>& getBiases();
	};

	template <typename Type>
	Layer<Type>::Layer(const int inputs, const int outputs):
	Weights(outputs, inputs), //tune these later depending on how dimensions in cuda should be n stuff
	Biases(1, outputs)
	{
	}


	template <typename Type>
	ArrayUtils::Array<Type> Layer<Type>::Forwards(ArrayUtils::Array<Type>& inputs)
	{
		/*
		 how much threads do we want you may ask? well you need a thread for each operation and we want to do an
		 operation between every input with all its coresponding weights, if we had 1 batch we would have to do the
		 amount of weights operations, eg with 1 batch we would have to add a input with its coresponding weights which
		 means its weight gets 1 operation, with 2 batches each weight would get 2 operations and etc

		 this means that we will be doing batches * weightsRows * weightsColumns operations so we should have each
		 block should have 10, 10, 10 threads so 1000 threads per block, also we should have batches be the z index,
		 y index be the neurons and x index be the inputs
		 */

		Type* outputPtr;
		cudaMalloc((void**) &outputPtr, sizeof(Type) * Weights.R * inputs.R);
		const int blockValSize = 10;
		const dim3 blockSize = dim3(blockValSize, blockValSize, blockValSize);
		/*
		 * so we need to put the inputs into groups of 10 in the grid, if we /10 then it rounds down bc of how
		 * int casting works so we have to add 9 to input columns basically the least inputs we could havfe is 1
		 * so with 1 input we would get 1/10 and that would go down to 0, but if we add 9 then it gets rounded up to
		 * 1 guaranteed bc (1+9)/10 = 1 meaning adding 1 to input.GetColumns will force a roundup every time
		 * all assuming that blockval size = 10, this concept can be applied to y and z too
		*/
		const unsigned int x = (inputs.GetColumns() + blockValSize - 1) / blockValSize;
		const unsigned int y = (Weights.GetRows() + blockValSize - 1) / blockValSize; //Weights.Rows is y axis which is neurons
		const unsigned int z = (inputs.GetRows() + blockValSize - 1) / blockValSize; //inputs rows is batches

		const dim3 gridDim(x, y, z);

		CudaFuncs::dotProduct <<<blockSize, gridDim>>> (inputs.D_Ptr, Weights.D_Ptr, outputPtr, Weights.GetRows(), inputs.GetRows(), inputs.GetColumns());
		cudaDeviceSynchronize();


		return ArrayUtils::Array<Type>(inputs.GetRows(), Weights.GetRows(), nullptr, outputPtr);

	}

	template <typename Type>
	void Layer<Type>::setWeights(ArrayUtils::Array<Type>& weights)
	{
		Weights = std::move(weights);
	}

	template <typename Type>
	void Layer<Type>::setBiases(ArrayUtils::Array<Type>& biases)
	{
		Biases = biases;
	}

	template <typename Type>
	const ArrayUtils::Array<Type>& Layer<Type>::getBiases()
	{
		return Biases;
	}
}

#endif