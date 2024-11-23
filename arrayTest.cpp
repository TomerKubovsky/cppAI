#include "array.h"

using namespace NeurelNetwork::ArrayUtils;

int main()
{
	Array<double> arr = Array<double>(10,10).customFunc([](double val, int index)
	{
		return 2.0;
	});

	Array<double> arr2 = Array<double>(10,10).customFunc([](double val, int index)
	{
		return 3.0;
	});

	arr.print();

	Array<double> newArrr = arr * arr2;
	newArrr.print();
}