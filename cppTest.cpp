#include <iostream>

using namespace std;

int* testFunc()
{   
    cerr << "test" << endl;
    throw std::invalid_argument("test123");
    int* h = new int[5];
    for (int i = 0; i < 5; i++)
    {
        *(h + i) = 1;
    }
    int* o = h;
    return o;
    delete h;
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

    for (int i = 0; i < 5; i++)
    {
        cout << "value pre disruption: " << *(ptr + i) << endl;
    }
    std::cout << "Address: " << ptr << std::endl;

    disruptStack(); // Call the function to disrupt the stack

    for (int i = 0; i < 5; i++)
    {
        cout << "value post disruption: " << *(ptr + i) << endl;
    }
    // delete h;
    delete ptr;
}
