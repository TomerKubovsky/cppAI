/*#include <cmath>
#include <matplot/matplot.h>
#include "neurelNetworkClass.h"

using namespace NeurelNetwork::ArrayUtils;

double function(double input)
{
    return input * input;
}

int main()
{
    const int layers[] = {1, 4, 4, 1};
    NeurelNetwork::neuralnetwork<double> neuralnetwork(layers, 4, "relu", "none", 0.01);

    const int size = 100;

    double* inputsPtr = new double[size];

    for (int i = 0; i < size; i++)
    {
        inputsPtr[i] = i;
    }

    double* trueY = new double[size];
    for (int i = 0; i < size; i++)
    {
        trueY[i] = function(inputsPtr[i]);
    }

   // for (int i = 0; i < size; i++)
   // {
   //     std::cout << trueY[i] << std::endl;
   // }

    // double* outputsPtr = new double[size];
    Array<double> inputsArr = Array<double>(inputsPtr, size, 1);
    // Array<double> outputsArr = Array<double>(outputsPtr, size, 1);

    Array<double> outputsArr = neuralnetwork.forwards(inputsArr);

    std::vector<double> outputsVec(outputsArr.getPtr(), outputsArr.getPtr() + outputsArr.getRows());

    std::vector<double> inputsVec(inputsPtr, inputsPtr + size );
    std::vector<double> trueYVec(trueY, trueY + size );

    matplot::plot(inputsVec, outputsVec, trueYVec, "b");
    matplot::show();
}*/

#include <matplot/matplot.h>
#include <chrono>
#include <thread>

int main() {
    using namespace matplot;

    // Create some initial data
    std::vector<double> x(100), y(100);
    for (size_t i = 0; i < 100; ++i) {
        x[i] = i;
        y[i] = sin(i * 0.1); // Initial y values (sine wave)
    }

    // Create a plot
    auto figure = plot(x, y);

    // Set plot title
    title("Dynamic Sine Wave");

    // Loop to update the graph at runtime
    for (int i = 0; i < 100; ++i) {
        // Update y values (change the sine wave over time)
        for (size_t j = 0; j < 100; ++j) {
            y[j] = sin((x[j] + i) * 0.1); // Shift sine wave
        }

        // Update the plot with the new data
        figure->y(y);

        // Redraw the plot
        redraw();

        // Wait for a short time (e.g., 100ms) before updating again
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
