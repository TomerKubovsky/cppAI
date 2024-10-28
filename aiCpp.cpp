#include "layerClass.h"
// #include <chrono>
#include <iostream>


template <typename Type>
class neuralnetwork
{
    Layer<Type>* Layers;

    int numOfLayers;

    Layer<Type>* generateLayers(const int layers[], const int sizeOfLayers, const std::string activationFuncHidden, const std::string activationFuncFinal);

public:
    neuralnetwork(const int layers[], const int sizeOfLayers, const std::string hiddenActivation, const std::string finalActivation);
    ~neuralnetwork();

    void printLayer(const int layerNum);

    Array<Type> forwards(const Array<Type>& inputs);
    void backwards(const Array<Type>& dInputs);
};

template<typename Type>
Layer<Type>* neuralnetwork<Type>::generateLayers(const int layers[], const int sizeOfLayers, const std::string activationFuncHidden, const std::string activationFuncFinal)
{
    const int numOfLayers = sizeOfLayers - 1;
    Layer<Type>* Layers = new Layer<Type>[numOfLayers];
    for (int index = 0; index < numOfLayers; index++) // we want it to loop through every layer but last one bc first layer inputs = first layer, layer outputs = index + 1 so we want to stop 1 before the last one to make the last one it makes sense to me rn
    {
        Layers[index] = Layer<Type>(layers[index], layers[index + 1], (index != numOfLayers - 1) ? activationFuncHidden : activationFuncFinal);
    }

    return Layers;
}

template<typename Type>
neuralnetwork<Type>::neuralnetwork(const int layers[], const int sizeOfLayers, const std::string hiddenActivation, const std::string finalActivation)
    :Layers(generateLayers(layers, sizeOfLayers, hiddenActivation, finalActivation)), numOfLayers(sizeOfLayers - 1)
{

}


template<typename Type>
neuralnetwork<Type>::~neuralnetwork()
{
    delete[] Layers;
}

template<typename Type>
void neuralnetwork<Type>::printLayer(const int layerNum)
{
    (Layers + layerNum)->print();
}

template<typename Type>
Array<Type> neuralnetwork<Type>::forwards(const Array<Type>& inputs)
{
    Array<Type> tempInputs = inputs.deepCopy();//here you could also just make the first layer run forwards on inputs and etc

    for (int layerIndex = 0; layerIndex < numOfLayers; layerIndex++)
    {
        tempInputs = Layers[layerIndex].forwards(tempInputs);
    }

    return tempInputs;
}

template<typename Type>
void neuralnetwork<Type>::backwards(const Array<Type>& dInputs)
{
    Array<Type> tempdInputs = dInputs.deepCopy();

    for (int layerIndex = numOfLayers; layerIndex >= 0; layerIndex--)
    {
        tempdInputs = Layers[layerIndex].backwards(tempdInputs);
    }

}


int main()
{
    try
    {

    } catch (testE e1)
    {

    }


    int layersVals[] = {1, 2, 3, 4};
    neuralnetwork<float> neurelNet(layersVals, 4, "relu", "sigmoid");

    float inputs[1][1] = {{6}};

    Array<float> inputsArr(&(inputs[0][0]), 1, 1);

    Array<float> outputs = neurelNet.forwards(inputsArr);

    outputs.print();

    float dInputs[1][4] = {{1, 1, 1, -1}};

    Array<float> dInputsArr(&(dInputs[0][0]), 1, 4);

    neurelNet.backwards(dInputsArr);
    //
    // outputs = neurelNet.forwards(inputsArr);
    //
    // outputs.print();

    return 0;
}
