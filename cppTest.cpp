#include <iostream>
#include <windows.h>
#include <cmath>

COLORREF interpolateRainbow(float percentage) {
    // Clamp percentage to [0, 1] range
    percentage = std::max(0.0f, std::min(1.0f, percentage));

    float hue = percentage * 360.0f; // Map percentage to hue (0 to 360 degrees)

    // Convert hue to RGB components
    float C = 1.0f; // Full saturation
    float X = C * (1 - std::fabs(fmod(hue / 60.0f, 2) - 1));
    float m = 0.0f;
    float r = 0, g = 0, b = 0;

    if (hue < 60) { r = C; g = X; b = 0; }
    else if (hue < 120) { r = X; g = C; b = 0; }
    else if (hue < 180) { r = 0; g = C; b = X; }
    else if (hue < 240) { r = 0; g = X; b = C; }
    else if (hue < 300) { r = X; g = 0; b = C; }
    else { r = C; g = 0; b = X; }

    // Convert each component to 0-255 range
    int R = static_cast<int>((r + m) * 255);
    int G = static_cast<int>((g + m) * 255);
    int B = static_cast<int>((b + m) * 255);

    // Use Windows API RGB function to create a COLORREF value
    return RGB(R, G, B);
}

int main()
{

    HDC dng = GetDC(NULL);

    const COLORREF testColorABC = RGB(255, 255, 255);

    // for (int i = 603; i < 1396; i+=48)
    // {
    //     std::cout << i << std::endl;
    // }

    // const COLORREF color = RGB(255, 0, 0);
    int h = 1;
    int h2 = 0;
    double j = 0;
    COLORREF c = GetPixel(dng, 10, 150);
    int n = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int x = 0 ; x < 1920 ; x++)
        {
            for (int y = 0; y < 1080 ; y++)
            {
                j = (((y + x * 1080)) / 20746.80)/100;
                n = j*255*(x+y+h2)*h;
                SetPixel(dng, x, y, interpolateRainbow(j));
                // if ((y + x * 1080) % 50 == 0)
                //     std::cout << n << std::endl;
            }
        }
    }

    return 0;
}