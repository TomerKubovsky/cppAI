#include "layerClass.h"
#include <chrono>
#include <iostream>
#include <set>

#include <typeinfo>

#include "neurelNetworkClass.h"

using namespace NeurelNetwork;

using namespace ArrayUtils;

int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    int layersVals[] = {1, 1};
    neuralnetwork<double> neurelNet(layersVals, 2, "leakyRelu", "none", -0.001, "adam");

    double* inputs = new double[100 * 1];

    for (int i = 0; i < 100; i++)
    {
        inputs[i] = -i;
    }



    Array<double> inputsArr(inputs, 100, 1);

    // Array<double> outputs = neurelNet.forwards(inputsArr);
    Array<double> outputs;
    // outputs.print();

    // double corrOutputsPtr[1][2] =
        // {{0, 1}};

    // Array<double> corrOutputsArr(&(corrOutputsPtr[0][0]), 1, 1);
    Array<double> corrOutputsArr(inputs, 100, 1);

    Array<double>* corrOutputsArrPtr = &(corrOutputsArr);
    // int numOfTimes = 5000000;
    int numOfTimes = 3;
    std::cout << std::setprecision(17);
    for (int index = 0; index < numOfTimes; index++)
    {
        outputs = neurelNet.forwards(inputsArr);
        outputs.print();
        // neurelNet.calculatedOutputs<Array<double>*>("mse", corrOutputsArrPtr).print();
        neurelNet.backwards(neurelNet.calculatedOutputs<Array<double>*>("mse", corrOutputsArrPtr));
        // neurelNet
        // neurelNet.getLayers()[0].getdWeights().print();
        // neurelNet.getLayers()[0].getWeights().print();
        // neurelNet.getLayers()[0].getBiases().print();
        // neurelNet.getLayers()[0].getdBiases().print();
        neurelNet.updateWeightsAndBiases();
        neurelNet.zeroGradient();
        // if (index % 1 == 0)
        // {
            // std::cout << "current index: " << index;
            // outputs.print();
            // std::cout << std::endl;
        // }
    }

    outputs.print();

    // neurelNet.getLayers()[0].getWeights().print();
    // neurelNet.getLayers()[0].getBiases().print();
    //
    // neurelNet.getLayers()[1].getWeights().print();
    // neurelNet.getLayers()[1].getBiases().print();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    std::cin.get()  ;

    // return 0;
}
