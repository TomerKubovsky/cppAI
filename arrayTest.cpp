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
	void print();

};

template <typename Type> Array<Type>::Array(int Rows, int Collumns)
{
	R = Rows;
	C = Collumns;

	ptr = new Type[R * C];
	
	for (int index = 0; index < R * C; index++)
	{
		ptr[index] = 5;
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
}

int main()
{
	Array<float> arr = Array<float>(3,1);

	arr.print();

}	