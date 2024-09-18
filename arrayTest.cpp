#include "array.hpp"

void disruptStack()
{
	int arrSize = 400000;
    int largeArray[arrSize]; // Larger array
    for (int i = 0; i < arrSize; ++i)
        largeArray[i] = i; // Force memory usage
}

int main()
{
	Array<float> testArr = Array<float>(2,3);

	disruptStack();

	testArr.print();

}