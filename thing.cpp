#include <iostream>

using namespace std;

int main()
{


	float *testArray[3];


	for (int index = 0; index < 3; index++)
	{
		testArray[index] = new float;
		*(testArray[index]) = 0.0f;
	}

	for (int index = 0; index < 3; index++)
	{
		cout << *(testArray[index]) << endl;
	}
}