#include <iostream>

int main()
{

    int j = 0;
    double h = 0;
    for (int x = 0 ; x < 1920 ; x++)
    {
        for (int y = 0; y < 1080 ; y++)
        {
            j = ((y + x * 1080));
            // h = ((y + x * 1080)) / 2074678;
            h = (((y + x * 1080)) / 20746.80)/100;
            // std::cout << h << "\n";

        }
        std::cout << h << "\n";
    }

    return 0;
}
