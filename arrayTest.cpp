#include <iostream>
#include <array>
using namespace std;

template <typename Type>
class Array
{

	Type* ptr;

	int R;
	int C;

public:

	Array(Type* arrPtr, int Rows, int Collumns);
	Array(int Rows, int Collumns);
	// Array<Type> multiply()
	void print();

};

template <typename Type> Array<Type>::Array(int Rows, int Collumns)
{
	R = Rows;
	C = Collumns;

	Type arrVals[R*C];

	for (int index = 0; index < R * C; index++)
	{
		arrVals[index] = 5;
		cout << &arrVals[index] << " next one: " << &arrVals[index]+1 << " thing at address: " << *(&arrVals[index] - 1)  << endl;
	}


	ptr = &arrVals[0];

}

template <typename Type> Array<Type>::Array(Type* arrPtr, int Rows, int Collumns)
{
	R = Rows;
	C = Collumns;


	ptr = arrPtr;
}

template <typename Type> void Array<Type>::print()
{
			// row * totalCollumns + collomn
	for (int index = 0; index < R * C; index++)
	{
		cout << ptr + index << endl;
		cout << *(ptr+index) << endl;
	}

}

int main()
{
	// float arrD[2][3] = {
	// 	{1.3f,4.51f,5.9512f},
	// 	{3.1f,0.0124f,9999.01f}
	// };
	// Array<float> arr = Array<float>(arrD[0],2,3);

	Array<float> arr = Array<float>(2,2);

	arr.print();
}


// collumn*R+row

// collumn=2
// R=2
// row=0

// 2*2+0=4
// 4