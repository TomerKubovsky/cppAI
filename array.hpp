#ifndef arrayUtils_H
#define arrayUtils_H

#include <random>
#include <iostream>
using namespace std;

template <typename Type>
class Array
{
	Type* ptr;


public:
	int R;
	int C;

public:
	Array(const Type* arrPtr, const int Rows, const int Collumns);
	Array(const int Rows, const int Collumns);
	void print() const;

	Type* dotProduct(const Type* inputArray, const int batches /*aka rows in input array*/) const; //takes an array of inputs and multiplys it with its own weights, the values in THIS array is the weights

	Type* Transpose() const; //returns transposed array
	
	~Array()
	{
		delete ptr;
	}

};

template <typename Type> Array<Type>::Array(const int Rows,const int Collumns)
{
    random_device rd;

    mt19937 gen(rd());

    uniform_real_distribution<> dis(0.0, 1.0);

	R = Rows;
	C = Collumns;

	ptr = new Type[R * C];
	
	for (int index = 0; index < R * C; index++)
	{
		ptr[index] = dis(gen);
	}

}

template <typename Type> Array<Type>::Array(const Type* arrPtr,const int Rows,const int Collumns)
{
	R = Rows;
	C = Collumns;
	ptr = arrPtr;
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


template <typename Type> Type* Array<Type>::Transpose() const
{
	Type* tempPtr = new Type[R*C];
	for (int row = 0; row < R; row++)
	{
		for (int collumn = 0; collumn < C; collumn++)
		{
			tempPtr[collumn * R + row] = ptr[row * C + collumn];
		}
	}

	return tempPtr;
}

template <typename Type> Type* Array<Type>::dotProduct(const Type* inputArray,const int batches /*aka rows in inputArray*/) const
{
	//amount of rows = amount of neurons so the amount of rows = the amount of collumns in the final array, the amount of rows in the input array = the amount of batches, so the amount of rows in input array = amount of rows in the output array

	
	Type* tempOutput;
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
				*(batchesOutputs + (batchNum * R + weightListNum)) += ptr[weightListNum * C + weightNum] * inputArray[batchNum * C + weightNum];			
			}
		}

	}


	return batchesOutputs;
}




#endif //end of arrayUtils_H