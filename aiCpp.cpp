#include "layerClass.h"
#include <chrono>
#include <iostream>

#include <typeinfo>

template <typename Type>
class neuralnetwork
{
    Layer<Type>* Layers;

    int numOfLayers;

    double learningRate = 0.001f;

    Layer<Type>* generateLayers(const int layers[], const int sizeOfLayers, const std::string activationFuncHidden, const std::string activationFuncFinal);

public:
    neuralnetwork(const int layers[], const int sizeOfLayers, const std::string hiddenActivation, const std::string finalActivation, Type learningRate);
    ~neuralnetwork();

    void printLayer(const int layerNum);

    Array<Type> forwards(const Array<Type>& inputs);
    Array<Type> forwards(Array<Type>&& inputs);
    void backwards(const Array<Type>& dInputs);

    void updateWeightsAndBiases();
    void zeroGradient();

    template <typename pointerType> Array<Type> calculatedOutputs(std::string lossFunc, pointerType extraDataVals);

    Layer<Type>* getLayers();
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
neuralnetwork<Type>::neuralnetwork(const int layers[], const int sizeOfLayers, const std::string hiddenActivation, const std::string finalActivation, Type learningRate)
    :Layers(generateLayers(layers, sizeOfLayers, hiddenActivation, finalActivation)), numOfLayers(sizeOfLayers - 1), learningRate(learningRate)
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
        tempInputs = Layers[layerIndex].forwards(std::move(tempInputs));
    }

    return tempInputs.deepCopy();
}

template<typename Type>
Array<Type> neuralnetwork<Type>::forwards(Array<Type>&& inputs)
{
    Array<Type> tempInputs = inputs;//here you could also just make the first layer run forwards on inputs and etc, also when you didnt do deep copy, what happened is you ended up calling delete on inputs and since input was created on stack you were deleting stack memrory

    for (int layerIndex = 0; layerIndex < numOfLayers; layerIndex++)
    {
        tempInputs = Layers[layerIndex].forwards(std::move(tempInputs));
    }

    return tempInputs.deepCopy();
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

template<typename Type>
void neuralnetwork<Type>::zeroGradient()
{
    for (int layerIndex = 0; layerIndex < numOfLayers; layerIndex++)
    {
        Layers[layerIndex].zeroGradient();
    }
}

template <typename Type>
template <typename pointerType>
Array<Type> neuralnetwork<Type>::calculatedOutputs(std::string lossFunc, pointerType extraDataVals)
{
    unsigned int batchCount = Layers[numOfLayers - 1].getOutputsPreActive().getRows();
    // int batchCount = 1;
    unsigned int outputCount = Layers[numOfLayers - 1].getOutputsPreActive().getColumns();

    Type* dOutputs = new Type[batchCount * outputCount]();

    if (lossFunc == "mse") //extra data vals is assumed to be an Array of type type with the correct / wanted outputs
    {

        if (std::is_same<pointerType, Array<Type>*>::value != true)
        {
            std::cerr << "didnt input Array pointer into calculate doutputs function in neurel network :( its type was: "  << typeid(pointerType).name() << " and " << typeid(extraDataVals).name() << std::endl;
        }

        Type* extraDataValsPtr = extraDataVals->getPtr();
        Type* selfOutputsPtr = Layers[numOfLayers - 1].getOutputsPostActive().getPtr();

        for (int batchIndex = 0; batchIndex < batchCount; batchIndex++) //technically using outputs pre active here isnt the best but the size of the batches and outputs doesnt change after the activation functions so it should be fine
        {
            for (int inputIndex = 0; inputIndex < outputCount; inputIndex++)
            {
                int index = inputIndex + batchIndex * outputCount;
                dOutputs[index] += (((extraDataValsPtr[index] - selfOutputsPtr[index]) * -2) / outputCount) / batchCount; //this is derivative for mean squared error, you divide by
            }
        }
    }

    return Array<Type>(dOutputs, batchCount, outputCount);
}

template<typename Type>
Layer<Type> * neuralnetwork<Type>::getLayers()
{
    return Layers;
}


int main()
{
    auto start = std::chrono::high_resolution_clock::now();

    int layersVals[] = {2, 1};
    neuralnetwork<double> neurelNet(layersVals, 2, "none", "none", -0.0001);

    double inputs[1][2] =
        {{1, 6}};

    Array<double> inputsArr(&(inputs[0][0]), 1, 2);

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
        // if (index % 1 == 0)
        // {
        //     std::cout << "current index: " << index;
        //     outputs.print();
        //     std::cout << std::endl;
        // }
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
