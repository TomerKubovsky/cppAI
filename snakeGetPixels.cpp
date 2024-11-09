#include <iostream>
#include <windows.h>

void fillColorefWithPixels(HDC dng, COLORREF* colors)
{
    unsigned int index = 0;
    for (int y = 311.5; y < 960; y+=44)
    {
        for (int x = 637.5; x < 1372; x+=44) //snake things are 46 pixels wide and tlal but they also have a 1 pixle thing on the right border and left
        {
            index++;
            SetPixel(dng, x, y, RGB(255, 0, 0));
            // for (int smallX = -1; smallX <= 1; smallX++)
            // {
            //     for (int smallY = -1; smallY <= 1; smallY++)
            //     {
            //             SetPixel(dng, x+smallX, y+smallY, RGB(255, 0, 0));
            //     }
            // }
            colors[index] = GetPixel(dng, x, y);
        }
    }
}


int main()
{
    HDC dng = GetDC(NULL);
    // for (int i = 0; i < 10000; i++)
        // fillColorefWithPixels(dng, NULL);

    COLORREF* colors = new COLORREF[255];
    fillColorefWithPixels(dng, colors);
    // for (int i = 0; i < 255; i++)
    // {
    //     std::cout << "index: " << i << " color: " << colors[i] << " " << (int) GetRValue(colors[i]) << " " << (int) GetGValue(colors[i]) << " " << (int) GetBValue(colors[i]) << std::endl;
    // }


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