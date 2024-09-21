// #include <iostream>

// int main()
// {
//     for (int i = 0; i < 10; i++)
//     {
//         std::cout << "hello!" << std::endl;
//     }
// }


#include <iostream>

int func(int h)
{
    
    if (h > 0)
    {
        std::cout << "hello!" << std::endl;
        return func(h-1);
    } else
    {
        return 0;
    }
}

int main()
{
    func(10);
}