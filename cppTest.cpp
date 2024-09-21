#include <iostream>

int* testFunc()
{
    int h = 6;
    int* o = &h;
    return o;
}

void disruptStack()
{
    int largeArray[10000]; // Larger array
    for (int i = 0; i < 10000; ++i)
        largeArray[i] = i; // Force memory usage
}

int main()
{
    int* ptr = testFunc();

    std::cout << "Value: " << *ptr << std::endl;
    std::cout << "Address: " << ptr << std::endl;

    disruptStack(); // Call the function to disrupt the stack

    std::cout << "Value after stack disruption: " << *ptr << std::endl;
}
