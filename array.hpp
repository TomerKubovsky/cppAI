#ifndef arrayUtils_H
#define arrayUtils_H
#include <random>
#include <iostream>
#include <chrono>
using namespace std;

template <typename Type>
class Array
{
	Type* ptr;
	int R;
	int C;

public:
	Array();//defult constructor shouldnt be used UNLESS you assign the array immedietly after, use with EXTREME CAUTION
	Array(Type* arrPtr, const int Rows, const int Collumns);
	Array(const int Rows, const int Collumns);
	static Array<Type>* constructArray(const int Rows, const int Collumns);
	void print() const;
	Array<Type> dotProduct(const Array<Type> inputArray) const; //takes an array of inputs and multiplys it with its own weights, the values in THIS array is the weights
	Array<Type> add(Array<Type> inputs) const;
	Array<Type> Transpose() const; //returns transposed array
	Array<Type> customFunc(Type (*func)(Type, int)) const;
	~Array() {delete[] ptr;}
	int GetRows() const;
	int GetCollumns() const;
	Type* GetPtr() const;



};

template <typename Type> Array<Type>::Array()
{
	R = 0;
	C = 0;
	ptr = nullptr;
}


template <typename Type> Array<Type>::Array(Type* arrPtr,const int Rows,const int Collumns)
{
	R = Rows;
	C = Collumns;
	ptr = arrPtr;
}

template <typename Type> Array<Type>::Array(const int Rows,const int Collumns)
{
    // mt19937 gen(chrono::system_clock::now().time_since_epoch().count());

    // uniform_real_distribution<> dis(0.0, 1.0);

	R = Rows;
	C = Collumns;

	ptr = new Type[R * C];
	
	for (int index = 0; index < R * C; index++)
	{
		// ptr[index] = dis(gen);
		// cout << "test" << endl;
		ptr[index] = 0.0f;
	}

}



template <typename Type> Array<Type>* Array<Type>::constructArray(const int Rows,const int Collumns)
{
	return new Array<Type>(Rows, Collumns);
}



template <typename Type> void Array<Type>::print() const
{

	for (int index = 0; index < R * C; index++)
	{
		if (index % C == 0)
		{
			cout << endl;
		} else
		{
			cout << ' ';
		}
		cout << *(ptr+index);
	}
	cout << endl;
}

template <typename Type> Array<Type> Array<Type>::dotProduct(const Array<Type> inputArray) const
{
	//amount of rows = amount of neurons so the amount of rows = the amount of collumns in the final array, the amount of rows in the input array = the amount of batches, so the amount of rows in input array = amount of rows in the output array

	if (inputArray.GetCollumns() != C)
	{
		throw std::invalid_argument("input array and self array collumns do not match in dot product");
	}

	const int batches = inputArray.GetRows();

	
	Type* batchesOutputs = new Type[batches * R];

	for (int index = 0; index < batches * R; index++)
	{
		*(batchesOutputs + index) = 0;
	}



	for (int batchNum /*aka row num for input array*/ = 0; batchNum < batches; batchNum++)
	{
		for (int weightListNum = 0; weightListNum < R; weightListNum++)
		{
			for (int weightNum = 0; weightNum < C; weightNum++)
			{
				*(batchesOutputs + (batchNum * R + weightListNum)) += ptr[weightListNum * C + weightNum] * *(inputArray.ptr + (batchNum * C + weightNum));			
			}
		}

	}
	/*
	this code is built to be like weights.DotProduct(inputs) where weights look like:
	neuron1Weight1, neuron1Weight2
	neuron2Weight1, neuron2Weight2
	and inputs:
	input1batch1, input2batch1
	input1batch2, input2batch2

	collumns have to match, rows dont
	*/
	return Array<Type>(batchesOutputs, inputArray.GetRows(), R);
}


template <typename Type> Array<Type> Array<Type>::add(Array<Type> inputs) const
{
	Type* outputs = new Type[R*C];
	Type* inputPtr = inputs.GetPtr();
	for (int index = 0; index < R*C; index++)
	{
		*(outputs + index) = *(ptr + index) + *(inputPtr + index);
	}

	return Array<Type>(outputs, R, C);
}


template <typename Type> Array<Type> Array<Type>::Transpose() const
{
	Type* tempPtr = new Type[R*C];
	for (int row = 0; row < R; row++)
	{
		for (int collumn = 0; collumn < C; collumn++)
		{
			tempPtr[collumn * R + row] = ptr[row * C + collumn];
		}
	}

	return Array<Type>(tempPtr, C, R); //rows and collumns are reversed after transpose
}

template <typename Type> Array<Type> Array<Type>::customFunc(Type (*func)(Type, int)) const
{
	Type* tempPtr = new Type[R*C];
	for (int index = 0; index < R*C; index++)
	{
		*(tempPtr + index) = func(*(ptr + index), index);
	}
	return Array<Type>(tempPtr, R, C);
}



template <typename Type> int Array<Type>::GetRows() const
{
	return R;
}

template <typename Type> int Array<Type>::GetCollumns() const
{
	return C;
}

template <typename Type> Type* Array<Type>::GetPtr() const
{
	return ptr;
}



#endif //end of arrayUtils_H




