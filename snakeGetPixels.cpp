#include <iostream>
#include <windows.h>


int main()
{
    HDC dng = GetDC(NULL);

    COLORREF* colors = new COLORREF[];


    for (int x = 602; x < 1372; x+=48) //snake things are 46 pixels wide and tlal but they also have a 1 pixle thing on the right border and left
    {
        for (int y = 285; y < 1000; y+=48)
        {
            // SetPixel(dng, x, y, RGB(255, 0, 0));
            GetPixel(dng, x, y);
        }
    }
    // COLORREF myColor = GetPixel(dng, 960, 540);
    //
    // std::cout << myColor << std::endl;


    return 0;
}

// for (int i = 0; i < 10000; i++)
// {
//     for (int x = 602; x < 1372; x+=48) //snake things are 46 pixels wide and tlal but they also have a 1 pixle thing on the right border and left
//     {
//         for (int y = 285; y < 1000; y+=48)
//         {
//             for (int smallX = -1; smallX <= 1; smallX++)
//             {
//                 for (int smallY = -1; smallY <= 1; smallY++)
//                 {
//                     SetPixel(dng, x+smallX, y+smallY, RGB(255, 0, 0));
//                 }
//             }
//         }
//     }
// }