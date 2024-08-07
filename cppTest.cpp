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
	// Array<Type> multiply()
	void print();

};


template <typename Type> Array<Type>::Array(Type* arrPtr[][], int Rows, int Collumns)
{
	R = Rows;
	C = Collumns;


	ptr = arrPtr;
}

template <typename Type> void Array<Type>::print()
{
	for (int collumn = 0; collumn < C; collumn++)
	{
		for (int row = 0; row < R; row++)
		{
			 cout << *(ptr + (collumn * R + row)) << endl;
		}
	}

}

int main()
{
	float arrD[2][3] = {
		
		{1.3f,4.51f,5.9512f},
		{3.1f,0.0124f,9999.01f}
	
	};
	Array<float> arr = Array<float>(arrD,2,3);

	arr.print();
}