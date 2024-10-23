#ifndef arrayUtils_H
#define arrayUtils_H
#include <iostream>

namespace ArrayUtils
{
	template <typename Type>
	class Array
	{
		Type* ptr;
		unsigned int R;
		unsigned int C;

	// public:
	// 	Type* ptr = NULL;
	// 	int R;
	// 	int C;

	public:
		Array();//defult constructor shouldnt be used UNLESS you assign the array immedietly after, use with EXTREME CAUTION
		Array(Type* arrPtr, const int Rows, const int Columns);
		Array(Type& arrPtr);
		Array(const int Rows, const int Columns);

		Array(Array<Type>&& otherArr/*other array is a temporary object that isa getting deleted*/);	//move constructor
		Array(const Array<Type>& otherArr);//copy constructor
		Array& operator=(const Array<Type>& arr);
		Array& operator=(Array<Type>&& arr); //move assignment operator
		static Array<Type>* constructArray(const int Rows, const int Columns);
		Array deepCopy() const;

		void print() const;
		void nullify();

		Array dotProduct(const Array<Type>& inputArray) const; //takes an array of inputs and multiplys it with its own weights, the values in THIS array is the weights
		Array operator+(const Array<Type>& arr);
		Array add(const Array<Type>& inputs) const;
		Array operator-(const Array<Type>& arr);
		Array subtract(const Array<Type>& inputs) const;
		Array multiply(Array<Type>& inputs) const;
		Array Transpose() const; //returns transposed array
		Array customFunc(Type (*func)(Type, int)) const;
		Array customFunc2Arr(const Array<Type>& arr2, Type (*func)(Type, Type, int)) const;

		~Array()
		{
			delete[] ptr;
		}

		int GetRows() const;
		int GetColumns() const;
		Type* GetPtr() const;



	};

	template <typename Type> Array<Type>::Array():
		ptr(nullptr),
		R(0),
		C(0)
	{}


	template <typename Type>
	Array<Type>::Array(Type* arrPtr,const int Rows,const int Columns):
		ptr(arrPtr),
		C(Columns),
		R(Rows)
	{}

	template <typename Type>
	Array<Type>::Array(Type& arrPtr)
	{
		*this = arrPtr;
	}

	template <typename Type>
	Array<Type>::Array(const int Rows,const int Columns):
		R(Rows),
		C(Columns),
		ptr(new Type[Rows*Columns]())//() makes it intiliaze at 0
	{
	    // mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
	    // uniform_real_distribution<> dis(0.0, 1.0);
		// for (int index = 0; index < R * C; index++)
		// {
		// 	// ptr[index] = dis(gen);
		// 	ptr[index] = 0.0f;
		// }

	}

	template <typename Type>
	Array<Type>::Array(Array<Type>&& otherArr):
		ptr(otherArr.ptr),
		R(otherArr.R),
		C(otherArr.C)
	{
		// std::cout << "move cconsturctor good" << std::endl;
		otherArr.ptr = nullptr;
		otherArr.R = 0;
		otherArr.C = 0;
	}

	template <typename Type>
	Array<Type>::Array(const Array<Type>& otherArr)
	{
		// std::cout << "copy cconstructor used, it is a shalow copy its probably fine" << std::endl;
		ptr = otherArr.ptr;
		R = otherArr.R;
		C = otherArr.C;
	}

	template <typename Type>
	Array<Type>& Array<Type>::operator=(const Array<Type>& arr)
	{

		// std::cout << "copy operator used" << std::endl;
		if (ptr != arr.ptr)
		{
			delete[] ptr;
		}
		C = arr.C;
		R = arr.R;
		ptr = arr.ptr;
		return *this;
	}

	template<typename Type>
	Array<Type> & Array<Type>::operator=(Array<Type> &&arr)
	{
		// std::cout << "move operator used, good" << std::endl;
		delete[] ptr;
		ptr = arr.ptr;
		R = arr.R;
		C = arr.C;

		arr.ptr = nullptr;
		return *this;
	}

	template <typename Type>
	Array<Type>* Array<Type>::constructArray(const int Rows,const int Columns)
	{
		return new Array<Type>(Rows, Columns);
	}

	template<typename Type>
	Array<Type> Array<Type>::deepCopy() const
	{
		Type* newArrPtr = new Type[R*C];
		for (int index = 0; index < R * C; index++)
		{
			newArrPtr[index] = ptr[index];
		}
		// return std::move(Array<Type>(newArrPtr, R, C));
		return Array<Type>(newArrPtr, R, C);
	}


	template <typename Type>
	void Array<Type>::print() const
	{

		for (int index = 0; index < R * C; index++)
		{
			if (index % C == 0)
			{
				std::cout << std::endl;
			} else
			{
				std::cout << ' ';
			}
			std::cout << *(ptr+index);
		}
		std::cout << std::endl;
	}

	template<typename Type>
	void Array<Type>::nullify()
	{
		ptr = nullptr;
		R = 0;
		C = 0;
	}

	template <typename Type>
	Array<Type> Array<Type>::dotProduct(const Array<Type>& inputArray) const
	{
		//amount of rows = amount of neurons so the amount of rows = the amount of columns in the final array, the amount of rows in the input array = the amount of batches, so the amount of rows in input array = amount of rows in the output array

		if (inputArray.C != C) {throw std::invalid_argument("input array and self array columns do not match in dot product");}

		const int& batches = inputArray.R;


		Type* batchesOutputs = new Type[batches * R]();



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

		columns have to match, rows dont
		*/
		return Array<Type>(batchesOutputs, inputArray.R, R);
	}

	template<typename Type>
	Array<Type> Array<Type>::operator+(const Array<Type> &arr)
	{
		Array<Type>& addedArrRef = add(arr);
		return Array<Type>(addedArrRef.ptr, addedArrRef.R, addedArrRef.C);
	}


	template <typename Type>
	Array<Type> Array<Type>::add(const Array<Type>& inputs) const
	{
		if (inputs.R != R) {throw std::invalid_argument("inputs rows is not equal to self rows in add arr");}
		if (inputs.C != C) {throw std::invalid_argument("inputs columns is not equal to self columns in add arr");}

		Type* outputs = new Type[R*C];
		Type* inputPtr = inputs.ptr;
		for (int index = 0; index < R*C; index++)
		{
			*(outputs + index) = *(ptr + index) + *(inputPtr + index);
		}

		return Array<Type>(outputs, R, C);
	}

	template<typename Type>
	Array<Type> Array<Type>::operator-(const Array<Type> &arr)
	{
		Array<Type>& subbedArrRef = subtract(arr);
		return Array<Type>(subbedArrRef.ptr, subbedArrRef.R, subbedArrRef.C);
	}

	template<typename Type>
	Array<Type> Array<Type>::subtract(const Array<Type>& inputs) const
	{
		if (inputs.R != R) {throw std::invalid_argument("inputs rows is not equal to self rows in subtract arr");}
		if (inputs.C != C) {throw std::invalid_argument("inputs columns is not equal to self columns in subtrct arr");}

		Type* outputs = new Type[R*C];
		Type* inputPtr = inputs.ptr;
		for (int index = 0; index < R*C; index++)
		{
			*(outputs + index) = *(ptr + index) - *(inputPtr + index);
		}

		return Array<Type>(outputs, R, C);
	}

	template <typename Type>
	Array<Type> Array<Type>::multiply(Array<Type>& inputs) const
	{
		if (inputs.R != R) {throw std::invalid_argument("inputs rows is not equal to self rows in mult arr");}
		if (inputs.C != C) {throw std::invalid_argument("inputs columns is not equal to self columns in mult arr");}

		Type* outputs = new Type[R*C];
		Type* inputPtr = inputs.ptr;
		for (int index = 0; index < R*C; index++)
		{
			*(outputs + index) = *(ptr + index) * *(inputPtr + index);
		}

		return new Array<Type>(outputs, R, C);
	}


	template <typename Type>
	Array<Type> Array<Type>::Transpose() const
	{
		Type* tempPtr = new Type[R*C];
		for (int row = 0; row < R; row++)
		{
			for (int collumn = 0; collumn < C; collumn++)
			{
				tempPtr[collumn * R + row] = ptr[row * C + collumn];
			}
		}

		return Array<Type>(tempPtr, C, R); //rows and columns are reversed after transpose
	}

	template <typename Type>
	Array<Type> Array<Type>::customFunc(Type (*func)(Type, int)) const
	{
		Type* tempPtr = new Type[R*C];
		for (int index = 0; index < R*C; index++)
		{
			*(tempPtr + index) = func(*(ptr + index), index);
		}
		return Array<Type>(tempPtr, R, C);
	}


	template <typename Type>
	Array<Type> Array<Type>::customFunc2Arr(const Array<Type>& arr2, Type (*func)(Type, Type, int)) const
	{
		if (arr2.R != R) {std::cerr << "arr2 rows do not equal arr1 rows in customFunc2Arr :(" << std::endl;}
		if (arr2.C != C) {std::cerr << "arr2 columns do not equal arr1 columns in customFunc2Arr :(" << std::endl;}

		Type* arr2Ptr = arr2.ptr; //pointer doesnt need to be delted as arr2 is responsible for controling that memory and has a destructor

		Type* tempPtr = new Type[R*C];
		for (int index = 0; index < R*C; index++)
		{
			*(tempPtr + index) = func(*(ptr+index), *(arr2Ptr + index), index);
		}
		return Array<Type>(tempPtr, R, C);
	}

	template <typename Type>
	int Array<Type>::GetRows() const
	{
		return R;
	}

	template <typename Type>
	int Array<Type>::GetColumns() const
	{
		return C;
	}

	template <typename Type>
	Type* Array<Type>::GetPtr() const
	{
		return ptr;
	}


}



#endif //end of arrayUtils_H




