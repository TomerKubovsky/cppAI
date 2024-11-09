#include <iostream>

int main()
{
    for (int i = 0; i < 100; i++)
    {
        std::cout << "f";
        for (int k = 0; k < i; k++)
        {
            std::cout << "'";
        }
        std::cout << "(0),";
    }
}