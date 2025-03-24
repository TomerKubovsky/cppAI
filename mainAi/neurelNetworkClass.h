#ifndef NEURELNETWORKCLASS_H
#define NEURELNETWORKCLASS_H

#include "layerClass.h"

namespace NeurelNetwork
{
    template <typename Type>
    class neuralnetwork
    {
        Layer<Type>* Layers;

        int numOfLayers;

        std::string optimizer;

        double learningRate = static_cast<Type>(0.001);

        Layer<Type>* generateLayers(const int layers[],  int sizeOfLayers,  std::string activationFuncHidden, std::string activationFuncFinall, std::string optimizer);

    public:
        neuralnetwork(const int layers[], const int sizeOfLayers, const std::string hiddenActivation, const std::string finalActivation, Type learningRate, std::string optimizer);
        ~neuralnetwork();

        void printLayer(const int layerNum);

        ArrayUtils::Array<Type> forwards(const ArrayUtils::Array<Type>& inputs);
        ArrayUtils::Array<Type> forwards(ArrayUtils::Array<Type>&& inputs);
        void backwards(const ArrayUtils::Array<Type>& dInputs);

        void updateWeightsAndBiases();
        void zeroGradient();

        template <typename pointerType> ArrayUtils::Array<Type> calculatedOutputs(std::string lossFunc, pointerType extraDataVals);

        Layer<Type>* getLayers();
    };

    template<typename Type>
    Layer<Type>* neuralnetwork<Type>::generateLayers(const int layers[], const int sizeOfLayers, const std::string activationFuncHidden, const std::string activationFuncFinal, const std::string optimizer)
    {
        const int numOfLayers = sizeOfLayers - 1;
        Layer<Type>* Layers = new Layer<Type>[numOfLayers];
        for (int index = 0; index < numOfLayers; index++) // we want it to loop through every layer but last one bc first layer inputs = first layer, layer outputs = index + 1 so we want to stop 1 before the last one to make the last one it makes sense to me rn
        {
            Layers[index] = Layer<Type>(layers[index], layers[index + 1], ((index != numOfLayers - 1) ? activationFuncHidden : activationFuncFinal), optimizer);
        }

        return Layers;
    }

    template<typename Type>
    neuralnetwork<Type>::neuralnetwork(const int layers[], const int sizeOfLayers, const std::string hiddenActivation, const std::string finalActivation, Type learningRate, std::string optimizer)
        :Layers(generateLayers(layers, sizeOfLayers, hiddenActivation, finalActivation, optimizer)), numOfLayers(sizeOfLayers - 1), learningRate(learningRate), optimizer(optimizer)
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
    ArrayUtils::Array<Type> neuralnetwork<Type>::forwards(const ArrayUtils::Array<Type>& inputs)
    {
        ArrayUtils::Array<Type> tempInputs = inputs.deepCopy();//here you could also just make the first layer run forwards on inputs and etc, also when you didnt do deep copy, what happened is you ended up calling delete on inputs and since input was created on stack you were deleting stack memrory

        for (int layerIndex = 0; layerIndex < numOfLayers; layerIndex++)
        {
            tempInputs = Layers[layerIndex].forwards(std::move(tempInputs));
        }

        return tempInputs.deepCopy();
    }

    template<typename Type>
    ArrayUtils::Array<Type> neuralnetwork<Type>::forwards(ArrayUtils::Array<Type>&& inputs)
    {
        ArrayUtils::Array<Type> tempInputs = inputs;//here you could also just make the first layer run forwards on inputs and etc, also when you didnt do deep copy, what happened is you ended up calling delete on inputs and since input was created on stack you were deleting stack memrory

        for (int layerIndex = 0; layerIndex < numOfLayers; layerIndex++)
        {
            tempInputs = Layers[layerIndex].forwards(std::move(tempInputs));
        }

        return tempInputs.deepCopy();
    }

    template<typename Type>
    void neuralnetwork<Type>::backwards(const ArrayUtils::Array<Type>& dInputs)
    {
        ArrayUtils::Array<Type> tempdInputs = dInputs.deepCopy();

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
    ArrayUtils::Array<Type> neuralnetwork<Type>::calculatedOutputs(std::string lossFunc, pointerType extraDataVals)
    {
        unsigned int batchCount = Layers[numOfLayers - 1].getOutputsPreActive().getRows();
        // int batchCount = 1;
        unsigned int outputCount = Layers[numOfLayers - 1].getOutputsPreActive().getColumns();

        Type* dOutputs = new Type[batchCount * outputCount]();

        if (lossFunc == "mse") //extra data vals is assumed to be an Array of type type with the correct / wanted outputs
        {

            if (std::is_same<pointerType, ArrayUtils::Array<Type>*>::value != true)
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
                    dOutputs[index] += (((extraDataValsPtr[index] - selfOutputsPtr[index]) * -2) / outputCount); //this is derivative for mean squared error, you divide by
                    //note for future me, idk if you are meant to divide by output count or by output count * batch count bc are u meant to averager it for all batches?
                }
            }
        }

        return ArrayUtils::Array<Type>(dOutputs, batchCount, outputCount);
    }

    template<typename Type>
    Layer<Type> * neuralnetwork<Type>::getLayers()
    {
        return Layers;
    }
}

#endif //NEURELNETWORKCLASS_H
