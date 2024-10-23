// #include <windows.h>
// #include <iostream>
//
//
// int main()
// {
//     // Assigning the device context to the current output device
//     HDC dng = GetDC(NULL);
//
//     // Calling the getpixel function and passing in the device context and coordinates
//     // The coordinates should be inside the max resolution of the device context
//     // ex. screen coordinate X < 1919; in case of a full HD display
//     COLORREF *colors = new COLORREF[200];
//
//     for (int i = 0; i < 200; i++)
//     {
//         colors[i] = GetPixel(dng, i, 1040);
//     }
//     // COLORREF c = GetPixel(dng, 100, 1040);
//
//     // Beautifying the data to output as a tuple containing the RGB values
//     // The GetRValue function returns the Red color value of the pixel
//     // The GetGValue function returns the Green color value of the pixel
//     // The GetBValue function returns the Blue color value of the pixel
//     for (int i = 0; i < 200; i++)
//     {
//         std::cout<<"(";
//         std::cout<<(int)GetRValue(colors[i])<<", ";
//         std::cout<<(int)GetGValue(colors[i])<<", ";
//         std::cout<<(int)GetBValue(colors[i])<<")\n";
//     }
//
//     // Releasing the Handle
//     ReleaseDC(NULL, dng);
// }