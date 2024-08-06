#include <iostream>
#include <iomanip>
#include <typeinfo>
using namespace std;


template <typename T, std::size_t Rows, std::size_t Collumns> class Array
{
    T* ptr;

public:
        // Array(); //defult constructor

        Array(T* arrayPtr); //constructor

        // Array(int arraySize); //constructor
        
        // int, int getSize();
        
        Array<T, Rows, Collumns> multiply(Array<T, Rows, Collumns>& multArray); //multiplication func

        // Array<T> divide(T divArray); //divide func

        // Array<T> add(T addArray); //add function 

        // Array<T> subtract(T subArray); //subtraction function

        void printArray();

};

// template <typename T> Array<T>::Array()
// {
//     return 
// }

template <typename T, std::size_t Rows, std::size_t Collumns> Array<T, Rows, Collumns>::Array(T* arrayPtr)
{
    ptr = new T[Rows * Collumns];

    int tempIndexCalc;

    for (int index1Dimension = 0; index1Dimension < Rows; index1Dimension++)
    {
        for (int index2Dimension = 0; index2Dimension < Collumns; index2Dimension++)
        {
            tempIndexCalc = index1Dimension * Collumns + index2Dimension;
            ptr[tempIndexCalc] = *(arrayPtr + tempIndexCalc);
        }
    }
    delete tempIndexCalc;
}


// template <typename T> Array<T>::Array(int arraySize)
// {
//     ptr = new float[arraySize];
//     size = arraySize;
    
//     // T arrayData[size];
    
//     // for (int i = 0; i < arraySize; i++)
//     // {
//     //     ptr[i] = arrayData[i];
//     // }
// }

// template <typename T> Array<T> Array<T>::multiply(Array<T>& multArray)
// {
//     Array<T[]> newArray(size);
//     for (int index = 0; index < size; index++)
//     {
//         newArray.ptr[index] = ptr[index] * multArray.ptr[index]; 
//     }
//     return newArray;
// }

template <typename T, std::size_t Rows, std::size_t Collumns> void Array<T, Rows, Collumns>::printArray()
{
    for (int index1Dimension = 0; index1Dimension < Rows; index1Dimension++)
    {
        for (int index2Dimension = 0; index2Dimension < Collumns; index2Dimension++)
        {
            cout << ptr[index1Dimension * Collumns + index2Dimension] << endl;
        }
    }
}

int main()
{
    Array<float,2,3> testArray(&(new float[2][3] {
        {3.651f,0.91248f,7.81243f},
        {5.3f,3.0f,1236456234.231212f}
    })[0]);
    // Array<Array<float>> testArray(new Array<float>(new float[3] {2.4f,312.4f,49.21234f},3),1);

    testArray.printArray();

    // Array<float> multArray(new float[3] {2.0f,2.0f,2.0f}, 3); 

    // Array<float> multedArray = multArray.multiply(testArray);

    // multedArray.printArray();

}