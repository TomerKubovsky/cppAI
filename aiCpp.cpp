#include "layerClass.h"
// #include <chrono>
#include <iostream>


template <typename Type>
class neuralnetwork
{
    Layer<Type>* Layers;

    int numOfLayers;

    float learningRate = 0.001f;

    Layer<Type>* generateLayers(const int layers[], const int sizeOfLayers, const std::string activationFuncHidden, const std::string activationFuncFinal);

public:
    neuralnetwork(const int layers[], const int sizeOfLayers, const std::string hiddenActivation, const std::string finalActivation);
    ~neuralnetwork();

    void printLayer(const int layerNum);

    Array<Type> forwards(const Array<Type>& inputs);
    void backwards(const Array<Type>& dInputs);

    void updateWeightsAndBiases();

    template <typename pointerType> Array<Type> calculatedOutputs(std::string lossFunc, pointerType* extraDataVals);
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
    Array<Type> tempInputs = inputs.deepCopy();//here you could also just make the first layer run forwards on inputs and etc, also when you didnt do deep copy, what happened is you ended up calling delete on inputs and since input was created on stack you were deleting stack memrory

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

    for (int layerIndex = numOfLayers - 1; layerIndex >= 0; layerIndex--)
    {
        tempdInputs = Layers[layerIndex].backwards(tempdInputs);
    }

}

template <typename Type>
void neuralnetwork<Type>::updateWeightsAndBiases()
{
    for (int layerIndex = 0; layerIndex < numOfLayers; layerIndex++)
    {
        Layers[layerIndex].updateWeightsAndBiases(learningRate);
    }
}

template <typename Type>
template <typename pointerType>
Array<Type> neuralnetwork<Type>::calculatedOutputs(std::string lossFunc, pointerType* extraDataVals)
{
    unsigned int batchCount = Layers[numOfLayers - 1].getOutputsPreActive().GetRows();
    unsigned int outputCount = Layers[numOfLayers - 1].getOutputsPreActive().GetColumns();

    Type* dOutputs = new Type[batchCount * outputCount]();

    if (lossFunc == "mse") //extra data vals is assumed to be an Array of type type with the correct / wanted outputs
    {
        if (std::is_same<pointerType, Array<Type>*>::value != true)
        {
            std::cerr << "didnt input Array pointer into calculate doutputs function in neurel network :( " << std::endl;
        }

        Type* extraDataValsPtr = extraDataVals->getPtr();
        Type* selfOutputsPtr = Layers[numOfLayers - 1].getOutputsPostActive();

        for (int batchIndex = 0; batchIndex < batchCount; batchIndex++) //technically using outputs pre active here isnt the best but the size of the batches and outputs doesnt change after the activation functions so it should be fine
        {
            for (int inputIndex = 0; inputIndex < outputCount; inputIndex++)
            {
                unsigned int index = inputIndex + batchIndex * outputCount;
                dOutputs[index] += (((extraDataValsPtr[index] - selfOutputsPtr[index]) * -2) / outputCount) / batchCount; //this is derivative for mean squared error, you divide by
            }
        }
    }
}


int main()
{

    int layersVals[] = {1, 2, 3, 4};
    neuralnetwork<float> neurelNet(layersVals, 4, "relu", "sigmoid");

    float inputs[1][1] = {{6}};

    Array<float> inputsArr(&(inputs[0][0]), 1, 1);

    Array<float> outputs = neurelNet.forwards(inputsArr);

    outputs.print();

    float dInputs[1][4] = {{1, 1, 1, -1}};

    Array<float> dInputsArr(&(dInputs[0][0]), 1, 4);

    neurelNet.backwards(dInputsArr);

    neurelNet.updateWeightsAndBiases();

    outputs = neurelNet.forwards(inputsArr);

    outputs.print();

    std::cout << "test" << std::endl;

    return 0;
}
