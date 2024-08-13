#include <iostream>
using namespace std;



int main()
{
	int a = 5;
	int* b = &a;

	*(b+3) = 3;


	cout << *(b+3) << endl;


}