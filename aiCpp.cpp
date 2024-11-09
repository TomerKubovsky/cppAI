#include "layerClass.h"
#include <chrono>
#include <iostream>

#include <typeinfo>

#include "neurelNetworkClass.h"

using namespace NeurelNetwork;

using namespace ArrayUtils;

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    int layersVals[] = {2, 1};
    neuralnetwork<double> neurelNet(layersVals, 2, "none", "softmax", -0.0001);

    double* inputs = new double[1 * 2];

    inputs[0] = 0.3;
    inputs[1] = 0.9;

    Array<double> inputsArr(inputs, 1, 2);

    Array<double> outputs = neurelNet.forwards(inputsArr);

    // outputs.print();

    double corrOutputsPtr[1][1] =
        {{19.8}};

    Array<double> corrOutputsArr(&(corrOutputsPtr[0][0]), 1, 1);

    Array<double>* corrOutputsArrPtr = &(corrOutputsArr);
    // int numOfTimes = 5000000;
    int numOfTimes = 500000;
    for (int index = 0; index < numOfTimes; index++)
    {
        neurelNet.backwards(neurelNet.calculatedOutputs<Array<double>*>("mse", corrOutputsArrPtr));
        neurelNet.updateWeightsAndBiases();
        neurelNet.zeroGradient();
        outputs = neurelNet.forwards(inputsArr);
        if (index % 1 == 0)
        {
            std::cout << "current index: " << index;
            outputs.print();
            std::cout << std::endl;
        }
    }

    // outputs.print();

    // neurelNet.getLayers()[0].getWeights().print();
    // neurelNet.getLayers()[0].getBiases().print();
    //
    // neurelNet.getLayers()[1].getWeights().print();
    // neurelNet.getLayers()[1].getBiases().print();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    std::cin.get();

    // return 0;
}
