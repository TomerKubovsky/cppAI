#include <cstdint>

#include "neurelNetworkClass.h"
#include <iostream>



using namespace NeurelNetwork;
using namespace ArrayUtils;

int main()
{
    const int Layers[] = {1, 4, 4, 4};
    neuralnetwork<float> network(Layers, 4, "relu", "softmax", 0.001);
    float* Inputs = new float[1];

    Inputs[0] = 1;

    Array<float> inputsArr(Inputs, 1, 1);

    Array<float> outputs = network.forwards(inputsArr);

    outputs.print();

    float dOutputs[4] = {9, 2, 1, -8};

    Array<float> dOutputsArr(&(dOutputs[0]), 4, 1);

    network.backwards(dOutputsArr);
    network.updateWeightsAndBiases();
    network.zeroGradient();

    outputs = network.forwards(inputsArr);

    outputs.print();

}
