#include <iostream>
#include <array>
using namespace std;

template <typename Type>
class Array
{
	Type* ptr;


public:
	int R;
	int C;

public:
	Array(Type* arrPtr, int Rows, int Collumns);
	Array(int Rows, int Collumns);
	void print();

	Type* dotProduct(Type* inputArray, int batches /*aka rows in input array*/); //takes an array of inputs and multiplys it with its own weights, the values in THIS array is the weights

	Type* Transpose(); //returns transposed array
	

};

template <typename Type> Array<Type>::Array(int Rows, int Collumns)
{
	R = Rows;
	C = Collumns;

	ptr = new Type[R * C];
	
	for (int index = 0; index < R * C; index++)
	{
		ptr[index] = index + 1;
	}

}

template <typename Type> Array<Type>::Array(Type* arrPtr, int Rows, int Collumns)
{
	R = Rows;
	C = Collumns;
	ptr = arrPtr;
}

template <typename Type> void Array<Type>::print()
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


template <typename Type> Type* Array<Type>::Transpose()
{
	Type* tempPtr = new Type[R*C];
	for (int row = 0; row < R; row++)
	{
		for (int collumn = 0; collumn < C; collumn++)
		{
              // [row][collumn] = [collumn][row]
			// cout << collumn * R + row << endl;
			tempPtr[collumn * R + row] = ptr[row * C + collumn];
		}
	}

	return tempPtr;
}

template <typename Type> Type* Array<Type>::dotProduct(Type* inputArray, int batches /*aka rows in inputArray*/)
{
	//amount of rows = amount of neurons so the amount of rows = the amount of collumns in the final array, the amount of rows in the input array = the amount of batches, so the amount of rows in input array = amount of rows in the output array

	
	Type tempOutput = 0;
	Type* batchesOutputs[batches * R];

	for (int batchNum /*aka row num for input array*/ = 0; batchNum < batches; batchNum++)
	{
		for (int weightListNum = 0; weightListNum < R; weightListNum++)
		{
			tempOutput = 0;
			for (int weightNum = 0; weightNum < C; weightNum++)
			{
				// cout << ptr[weightListNum * C + weightNum] << endl;
				tempOutput += ptr[weightListNum * C + weightNum] + inputArray[batchNum * C + weightNum];			
			}
			batchesOutputs[batchNum * R + weightListNum] = tempOutput;
		}

	}

	return &(batchesOutputs[0][0]);
}



int main()
{
	Array<float> arr = Array<float>((&(new float[2][3]
		{
			{1.0f,2.0f,3.0f},
			{4.0f,5.0f,6.0f}
		})[0][0]),2,3);
	// Array<float> arr = Array<float>(2,3);


	// arr.print();

	// Array<float> tArr = Array<float>(arr.Transpose(),arr.C,arr.R);

	float inputs[1][3] = {{10.0f,10.0f,10.0f}};

	Array<float> mArr = Array<float>(arr.dotProduct(&(inputs[0][0]),1),1,3);


	// tArr.print();

}	