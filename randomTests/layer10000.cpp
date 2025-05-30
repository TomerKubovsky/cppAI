#include "layerClass.h"
#include <chrono>
#include <iostream>

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    Layer<float> testLayer(3,2, "linear");

    // testLayer.print();

    float inputData[2][3] = {{3.0f,2.0f,5.0f},
                                {2.0f, 2.0f, 2.0f}};

    Array<float> Inputs =  Array<float>(&(inputData[0][0]),2,3); //1 batch 3 inputs for that 1 batch

    Array<float> Outputs = testLayer.forwards(Inputs);

    // std::cout << "outputs:";
    // Outputs.print();

    float correctOutputsData[2][2] = {{35, 13},
                                    {19, -15}};

    Array<float> correctOutputs(&(correctOutputsData[0][0]), 2, 2);

    Array<float>* dOutputs;

    for (int i = 0; i < 10000; i++)
    {
        float* dOutputsData = new float[4];
        for (int rowIndex = 0; rowIndex < Outputs.getRows(); rowIndex++)
        {
            for (int colIndex = 0; colIndex < Outputs.getColumns(); colIndex++)
            {
                *(dOutputsData + (colIndex + rowIndex * Outputs.getColumns())) = ((correctOutputsData[rowIndex][colIndex] - *(Outputs.getPtr() + (colIndex + rowIndex * Outputs.getColumns()))) * 2/Outputs.getColumns());
            }
        }
        dOutputs = new Array<float>(dOutputsData, 2, 2);
        testLayer.backwards(*dOutputs);
        testLayer.updateWeightsAndBiases(1);
        testLayer.zeroGradient();
        Outputs = testLayer.forwards(Inputs);
        // std::cout << "iteration: " << i << std::endl << "outputs: ";
        // Outputs.print();
        delete dOutputs;
    }

    // delete[] dOutputsData;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    // Sleep(5000);

    // system("pause");

    return 0;
}