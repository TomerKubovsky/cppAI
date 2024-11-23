
#ifndef LAYERCLASS_H
#define LAYERCLASS_H

#include <cmath>
#include <complex>
#include <iostream>
#include <random>
#include <chrono>
#include "array.h"


// using namespace ArrayUtils::ArrayUtils; //bad practice but it's fine for now


namespace NeurelNetwork
{
    template <typename Type>
    class Layer
    {
    private:
        ArrayUtils::Array<Type> weights;
        ArrayUtils::Array<Type> biases;

        ArrayUtils::Array<Type> dWeights;
        ArrayUtils::Array<Type> dBiases;

        ArrayUtils::Array<Type> outputsPreActive;
        ArrayUtils::Array<Type> outputsPostActive;
        ArrayUtils::Array<Type> selfInputs;

        std::string activationFunc;
        std::string optimizer;

        ArrayUtils::Array<Type> weightMomentums;
        ArrayUtils::Array<Type> weightCache;
        ArrayUtils::Array<Type> biasMomentums;
        ArrayUtils::Array<Type> biasCache;
        Type beta1 = static_cast<Type>(0.9);
        Type beta2 = static_cast<Type>(0.999);
        Type epsilon = static_cast<Type>(0.0000001);
        int iterations = 0;
    public:
        Layer(int Inputs, int outputs /*outputs = neurons*/, std::string activationFunc = "none", std::string optimizer = "none");
        Layer();
        Layer(const Layer<Type>& other);
        Layer& operator=(const Layer<Type>& other);
        Layer(Layer<Type>&& other) noexcept ;
        Layer& operator=(Layer<Type>&& other) noexcept ;
        ~Layer();
        void print() const;

        ArrayUtils::Array<Type> forwards(const ArrayUtils::Array<Type>& Inputs);
        ArrayUtils::Array<Type> forwards(ArrayUtils::Array<Type>&& Inputs);
        ArrayUtils::Array<Type> forwardsActivation(const ArrayUtils::Array<Type>& Inputs) const;
        ArrayUtils::Array<Type> backwardsActivation(ArrayUtils::Array<Type>& dOutputs) const;
        ArrayUtils::Array<Type> backwards(ArrayUtils::Array<Type>& dOutputs) const; //doutputs isnt const because we use a mvoe constrcutor on it and nullify it after we dont need it anymore

        void updateWeightsAndBiases(Type learningRate);
        void zeroGradient();

        void setWeights(ArrayUtils::Array<Type>& weightVals);
        void setBiases(ArrayUtils::Array<Type>& biasVals);
        const ArrayUtils::Array<Type>& getWeights() const;
        const ArrayUtils::Array<Type>& getdWeights() const;
        const ArrayUtils::Array<Type>& getBiases() const;
        const ArrayUtils::Array<Type>& getdBiases() const;
        const ArrayUtils::Array<Type>& getOutputsPreActive() const;
        const ArrayUtils::Array<Type>& getOutputsPostActive() const;
        const ArrayUtils::Array<Type>& getSelfInputs() const;
        std::string getActivationFunc() const;


    };




    template <typename Type> Layer<Type>::Layer(const int Inputs, const int outputs, std::string activationFunc, std::string optimizer)
        :
        weights(ArrayUtils::Array<Type>(outputs, Inputs).customFunc([](Type input, int index)
            {
                // return static_cast<Type>(0.1);
                // return static_cast<Type>(index * 0.01);
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::mt19937 rng(seed);
                std::normal_distribution<> distribution(0.0, 1.0);
                return static_cast<Type>(distribution(rng) * 1);
            })),
        biases(ArrayUtils::Array<Type>(1, outputs).customFunc([](Type input, int index)
            {
                // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                // std::mt19937 rng(seed);
                // std::normal_distribution<> distribution(0.0, 1.0);
                // return static_cast<Type>(distribution(rng));
                return static_cast<Type>(0);
            })),
        dWeights(ArrayUtils::Array<Type>(outputs, Inputs)),
        dBiases(ArrayUtils::Array<Type>(1, outputs)),
        // biases(ArrayUtils::Array<Type>(1, outputs)), //1 bias for each neuron and there should be a bias for each neuron and outputs = num of neurons
        activationFunc(std::move(activationFunc)),
        optimizer(std::move(optimizer))
        // {{neuron1weight1, neuron1weight2, neuron1weight3},
        //  {neuron2weight1, neuron2weight2, neuron2weight3}} ammount of rows = amount of outputs, amount of columns = amount of inputs
    {
        if (this->optimizer == "adam")
        {
            weightMomentums = ArrayUtils::Array<Type>(outputs, Inputs);
            weightCache = ArrayUtils::Array<Type>(outputs, Inputs); //should be 0 size of weights same as above
            biasMomentums = ArrayUtils::Array<Type>(1, outputs);
            biasCache = ArrayUtils::Array<Type>(1, outputs);//should be 0s like biases
        }
    }

    template <typename Type>
    Layer<Type>::Layer()
    = default;

    template <typename Type>
    Layer<Type>::Layer(const Layer<Type>& other)
        : weights(other.weights), biases(other.biases), dWeights(other.weights), dBiases(other.biases), outputsPreActive(other.outputsPreActive), selfInputs(other.selfInputs), activationFunc(other.activationFunc)
    {
        // weights = other.getWeights();
        // biases = other.getBiases();
        // dWeights = other.getdWeights();
        // dBiases = other.getdBiases();
        // outputsPreActive = other.getOutputsPreActive();
        // selfInputs = other.getSelfInputs();
        // activationFunc = other.getActivationFunc();
    }

    template <typename Type>
    Layer<Type>& Layer<Type>::operator=(const Layer<Type>& other) = default;
    // {
    //     weights = other.weights;
    //     biases = other.biases;
    //     dWeights = other.dWeights;
    //     dBiases = other.dBiases;
    //     outputsPreActive = other.outputsPreActive;
    //     selfInputs = other.selfInputs;
    //     activationFunc = other.activationFunc;
    //
    //     return *this;
    // }

    template <typename Type>
    Layer<Type>::Layer(Layer<Type>&& other) noexcept
        // : weights(std::move(other.weights)), biases(std::move(other.biases)), dWeights(std::move(other.weights)),
        //   dBiases(std::move(other.biases)), outputsPreActive(std::move(other.outputsPreActive)), selfInputs(std::move(other.selfInputs)),
        //   activationFunc(std::move(other.activationFunc))
    {
        weights = std::move(other.weights);
        biases = std::move(other.biases);
        dWeights = std::move(other.dWeights);
        dBiases = std::move(other.dBiases);
        outputsPreActive = std::move(other.outputsPreActive);
        selfInputs = std::move(other.selfInputs);
        activationFunc = std::move(other.activationFunc);
    }

    template <typename Type>
    Layer<Type>& Layer<Type>::operator=(Layer<Type>&& other) noexcept
    {
        weights = std::move(other.weights);
        biases = std::move(other.biases);
        dWeights = std::move(other.dWeights);
        dBiases = std::move(other.dBiases);
        outputsPreActive = std::move(other.outputsPreActive);
        selfInputs = std::move(other.selfInputs);
        activationFunc = std::move(other.activationFunc);
        optimizer = std::move(other.optimizer);

        weightMomentums = std::move(other.weightMomentums);
        weightCache = std::move(other.weightCache);
        biasMomentums = std::move(other.biasMomentums);
        biasCache = std::move(other.biasCache);
        return *this;
    }


    template <typename Type>
    Layer<Type>::~Layer()
    = default;

    template <typename Type> void Layer<Type>::print() const
    {
        std::cout << "weights: ";
        weights.print();
        std::cout << "biases: ";
        biases.print();
    }

    template <typename Type>
    ArrayUtils::Array<Type> Layer<Type>::forwards(const ArrayUtils::Array<Type>& Inputs)
    {
        //std move cant work here bc it is a const object
        selfInputs = Inputs.deepCopy(); //std move is used here because this is called from neurel network class and tempinputs is deepcopied and thius isnt needed
        // selfInputs = Inputs.deepCopy(); //std move is used here because this is called from neurel network class and tempinputs is deepcopied and thius isnt needed
        ArrayUtils::Array<Type> outputs = weights.dotProduct(Inputs);//caclulate inputs * weights

        //add biases to output
        const int outputsCollumns = outputs.getColumns();
        const int outputRows = outputs.getRows();

        Type* outputsPreActivePtr = new Type[outputs.getColumns() * outputs.getRows()];
        Type* biasesPtr = biases.getPtr();
        for (int rowIndex = 0; rowIndex < outputRows; rowIndex++)
        {
            for (int collumnIndex = 0; collumnIndex < outputsCollumns; collumnIndex++)
            {
                outputsPreActivePtr[collumnIndex + outputsCollumns * rowIndex] = outputs.getPtr()[collumnIndex + outputsCollumns * rowIndex] + biasesPtr[collumnIndex];
            }
        }
        outputsPreActive = ArrayUtils::Array<Type>(outputsPreActivePtr, outputRows, outputsCollumns);
        // ArrayUtils::Array<Type> finalOutput = ForwardsActivation(outputsPreActive);//activation function
        outputsPostActive = std::move(forwardsActivation(outputsPreActive));
        return outputsPostActive.deepCopy(); //if this isnt deepcopied then essentialy what happens is it gets assigned to the tempinputs woith a copy constructer and then tempinputs later gets deleted but outputspostactive pointer isnt null meaning later in the code when the move constructor is called on it (in the line above) u get a double pointer deleteion and or undefined behaviour
    }

    template <typename Type>
    ArrayUtils::Array<Type> Layer<Type>::forwards(ArrayUtils::Array<Type>&& Inputs)
    {
        //std move cant work here bc it is a const object
        selfInputs = std::move(Inputs); //std move is used here because this is called from neurel network class and tempinputs is deepcopied and thius isnt needed
        // selfInputs = Inputs.deepCopy(); //std move is used here because this is called from neurel network class and tempinputs is deepcopied and thius isnt needed
        ArrayUtils::Array<Type> outputs = weights.dotProduct(selfInputs);//caclulate inputs * weights

        //add biases to output
        const int outputsCollumns = outputs.getColumns();
        const int outputRows = outputs.getRows();

        Type* outputsPreActivePtr = new Type[outputs.getColumns() * outputs.getRows()];
        Type* biasesPtr = biases.getPtr();
        for (int rowIndex = 0; rowIndex < outputRows; rowIndex++)
        {
            for (int collumnIndex = 0; collumnIndex < outputsCollumns; collumnIndex++)
            {
                outputsPreActivePtr[collumnIndex + outputsCollumns * rowIndex] = outputs.getPtr()[collumnIndex + outputsCollumns * rowIndex] + biasesPtr[collumnIndex];
            }
        }
        outputsPreActive = ArrayUtils::Array<Type>(outputsPreActivePtr, outputRows, outputsCollumns);
        // Array<Type> finalOutput = ForwardsActivation(outputsPreActive);//activation function
        outputsPostActive = std::move(forwardsActivation(outputsPreActive));
        return outputsPostActive.deepCopy(); //if this isnt deepcopied then essentialy what happens is it gets assigned to the tempinputs woith a copy constructer and then tempinputs later gets deleted but outputspostactive pointer isnt null meaning later in the code when the move constructor is called on it (in the line above) u get a double pointer deleteion and or undefined behaviour
    }
    template <typename Type>
    ArrayUtils::Array<Type> Layer<Type>::forwardsActivation(const ArrayUtils::Array<Type>& Inputs) const
    {
        if (activationFunc == "relu")
        {
            return Inputs.customFunc([](Type input, int index)
            {
                return (input > 0) ? input : 0;
            });
            //makes a lambda function for relu and passess it into inputs custom func
        } else if (activationFunc == "leakyRelu")
        {
            return Inputs.customFunc([](Type input, int index)
            {
                return (input > 0.1 * input) ? input : 0.1 * input; //could maybe be optimized by calculating input * 0.1 once and setting ti const?
            });
        } else if (activationFunc == "softmax")
        {
            const unsigned int inputRows = Inputs.getRows();
            const unsigned int inputColumns = Inputs.getColumns();
            Type* exponentiatedVals = new Type[inputRows * inputColumns];
            Type* addedVals = new Type[inputRows]();


            int currentIndex = 0;
            for (int rowIndex = 0; rowIndex < inputRows; rowIndex++)
            {
                for (int columnIndex = 0; columnIndex < inputColumns; columnIndex++)
                {
                    currentIndex = columnIndex + rowIndex * inputColumns;
                    exponentiatedVals[currentIndex] = std::exp(Inputs.getPtr()[currentIndex]);
                    addedVals[rowIndex] += exponentiatedVals[currentIndex];
                }
            }

            Type* softmaxedVals = new Type[inputRows * inputColumns];

            for (int rowIndex = 0; rowIndex < inputRows; rowIndex++)
            {
                for (int columnIndex = 0; columnIndex < inputColumns; columnIndex++)
                {
                    currentIndex = columnIndex + rowIndex * inputColumns;
                    softmaxedVals[currentIndex] = exponentiatedVals[currentIndex] / addedVals[rowIndex];
                }
            }
delete[] exponentiatedVals; delete[] addedVals;
            return ArrayUtils::Array<Type>(softmaxedVals, inputRows, inputColumns);
        } else
        {
            return Inputs.deepCopy();
        }
    }


    /*
        dactiv is the derivative of the inputs to the activbation functions so self.activationBackwards(dOutputs) also you gotta multiply it by dOutputs which is why it is passed into the function
    */
    template <typename Type> ArrayUtils::Array<Type> Layer<Type>::backwardsActivation(ArrayUtils::Array<Type>& dOutputs) const //this should return an array with douputs rows batches rows and doutputs collumns collumsn and columns is neurin num
    {
        if (activationFunc == "relu")
        {
            return outputsPreActive.customFunc2Arr(dOutputs, [](Type preActivArrVal, Type dOutputsArrVal, int index)
            {
                // return ((preActivArrVal > 0) ? 1 : 0) * dOutputsArrVal;
                return (preActivArrVal > 0) ? dOutputsArrVal : 0; //these two lines of code do the EXACT SAME THING just bottom one is faster and looks nicer, top one easier to understand tho
            });
        } else if (activationFunc == "leakyRelu")
        {
            return outputsPreActive.customFunc2Arr(dOutputs, [](Type preActivArrVal, Type dOutputsArrVal, int index)
            {
                // return ((preActivArrVal > 0) ? 1 : 0.1) * dOutputsArrVal;
                return (preActivArrVal > 0) ? dOutputsArrVal : 0.1 * dOutputsArrVal; //these two lines of code do the EXACT SAME THING just bottom one is faster and looks nicer, top one easier to understand tho
            });

        } else if (activationFunc == "softmax")
        {
		    const unsigned int* chosenValue = new unsigned int[dOutputs.getRows()](); //for now
		    Type* dActive = new Type[dOutputs.getRows() * dOutputs.getColumns()];

            const Type* selfInputsPtr = selfInputs.getPtr();

            for (int batchNum = 0; batchNum < dOutputs.getRows(); batchNum++)
            {
                for (int neuronNum = 0; neuronNum < dOutputs.getColumns(); neuronNum++)
                {
                    const unsigned int index = batchNum * dOutputs.getColumns() + neuronNum;
                    if (neuronNum == chosenValue[batchNum])
                    {
                        dActive[index] = selfInputsPtr[index] * (1 - selfInputsPtr[index]);
                    } else
                    {
                        dActive[index] = -selfInputsPtr[batchNum * dOutputs.getColumns() + chosenValue[batchNum]] * selfInputsPtr[index];
                    }
                }
            }
            return ArrayUtils::Array<Type>(dActive, dOutputs.getRows(), dOutputs.getColumns());
        } else
        {
            // return dOutputs.deepCopy(); //linear activation function derivative is 1 so 1*doutpus so doutputs
            return std::move(dOutputs); //linear activation function derivative is 1 so 1*doutpus so doutputs
        }
    }


    template <typename Type>
    ArrayUtils::Array<Type> Layer<Type>::backwards(ArrayUtils::Array<Type>& dOutputs /*dOutputs = derivative of outputs*/) const
    {
        /*
        weights derivatives = inputs since y=wx where x is input and w is weights, and derivative of y=wx with respect to w is x, this also means derivative of inputs is weights since d/dx y=wx is w
        bias deivatives = 1 since the equation is activationFunc(input1*weight1+input2*weight2+bias) and since we are deriviving with respect to bias everything else can be treated as constants
        furthermore with bias derivatives instead of writing it as  d                                         we can write it as   d  below value is 0 bc treated as a constant   d   the one below is 1 since y=x wrt x is 1 and this is y=bias wrt bias =
                                                                   ----(input1*weight1+input2*weight2+bias)                       ---- (input1*weight1).......                   ----bias
                                                                   dbias                                                          dbias                                          dbias
        remember all the derivatives must be multiplied by dOutputs due to chain rule



        //theoretical inputs
        1, 2, 3

        //theoritcal weights, 3 neurons
        1.3, 2.6, 3.8
        6.3, 9.6, 1.8
        7.3, 1.1, 3.6

        //derivatives of weights: add for all batches
        1 * dActivationFunc[0], 2 * dActivationFunc[0], 3 * dActivationFunc[0],
        1 * dActivationFunc[1], 2 * dActivationFunc[1], 3 * dActivationFunc[1] etc, if its multiple batches then you have to do 1 * dActivationFunc[batch1][0] + batch2input1 * dActivationFunc[batch2][0]

        can be written as inputs.transposed.dotproduct(dActivationFunc.transpose()) which can be seen as:
        let dactivationfunc = neuron1:7, neuron2:8, neuron3:9
        transformed into 7,8,9
                        transposed
        1            7
        2            8
        3            9

        eg:
        1*7,2*7,3*7
        1*8,2*8,3*8
        1*9,2*9,3*9

        with batches it would be addedd together so eg for the first neuron assuming that for the batches the derivative for neuron 1 is 56 etc
        1*7+17*56,2*7+18*56,3*7+21*56

        dot product above gives:
        1*7, 2*7, 3*7
        1*8, 2*8, 3*8
        1*9, 2*9, 3*9

        with multiple batches dotproducts add them together


        dactiv is the derivative of the inputs to the activbation functions so self.activationBackwards(dOutputs) also you gotta multiply it by dOutputs which is why it is passed into the function


        //derivatives of inputs =
        1.3 + 6.3 + 7.3, 2.6 + 9.6 + 1.1, 3.8 + 1.8 + 3.6*/

        //define dActive to be self.activationBackwards()
        const ArrayUtils::Array<Type> dActiv = backwardsActivation(dOutputs); //this gets deleted its temporary
        // dWeights = selfInputs.Transpose().dotProduct(dActiv.Transpose());

        Type* dWeightsPtr = dWeights.getPtr();
        const Type* inputPtr = selfInputs.getPtr();
        const Type* dActivPtr = dActiv.getPtr();
        Type* dInputsPtr = new Type[selfInputs.getRows() * selfInputs.getColumns()]();//new[a]() makes all values initialize at 0 new[a] makes it have random memory values
        const Type* weightsPtr = weights.getPtr();
        const int inputSize = selfInputs.getColumns();

        Type* dBiasPtr = dBiases.getPtr();
        const int neuronAmount = dActiv.getColumns();

        for (int batchNum = 0; batchNum < selfInputs.getRows(); batchNum++)
        {
            for (int neuronNum = 0; neuronNum < neuronAmount; neuronNum++)
            {
                for (int inputNum = 0; inputNum < selfInputs.getColumns(); inputNum++)
                {
                    dWeightsPtr[inputNum + neuronNum * inputSize] += inputPtr[inputNum + batchNum * inputSize] * dActivPtr[neuronNum + batchNum * neuronAmount]; //derivative of weight is the input of that batch and weight * derivative of the weights neuron for that batch
                    dInputsPtr[inputNum + batchNum * inputSize] += weightsPtr[inputNum + neuronNum * inputSize] * dActivPtr[neuronNum + batchNum * neuronAmount]; //derivative of input for that batch is the weight * derivative fo that weights neuron for that batch
                }

                dBiasPtr[neuronNum] += dActivPtr[neuronNum + batchNum * neuronAmount];
            }
        }

        return ArrayUtils::Array<Type>(dInputsPtr, selfInputs.getRows(), selfInputs.getColumns());

    }

    template<typename Type>
    void Layer<Type>::updateWeightsAndBiases(Type learningRate)
    {
        if (optimizer == "adam")
        {
            weightMomentums = weightMomentums * beta1 + dWeights * (1 - beta1);
            biasMomentums = biasMomentums * beta1 + dBiases * (1 - beta1);

            ArrayUtils::Array<Type> weightMomentumsCorrected = weightMomentums * (1 / (1 - std::pow(beta1, iterations + 1))); //this should be weights / x but since i dont have / as a operator func in array.h its easier to do weights * (1/x)
            ArrayUtils::Array<Type> biasMomentumsCorrected = biasMomentums * (1 / (1 - std::pow(beta1, iterations + 1)));

            weightCache =  weightCache * beta2 + dWeights * dWeights * (1 - beta2); //meant to be dweights ^ 2 but easier to do dweights * dweights
            biasCache =  biasCache * beta2 + dBiases * dBiases * (1 - beta2);

            ArrayUtils::Array<Type> weightCacheCorrected = weightCache * (1 / (1 - std::pow(beta2, iterations + 1)));
            ArrayUtils::Array<Type> biasCacheCorrected = biasCache * (1 / (1 - std::pow(beta2, iterations + 1)));

            ArrayUtils::Array<Type> weightsCacheRooted = weightCacheCorrected.customFunc([&](Type thisVal, int index)
            {
                return std::sqrt(thisVal) + epsilon;
            });
            ArrayUtils::Array<Type> biasCacheRooted = biasCacheCorrected.customFunc([&](Type thisVal, int index)
            {
                return std::sqrt(thisVal) + epsilon;
            });

            weights = weights + ((weightMomentumsCorrected.divide(weightsCacheRooted)) * learningRate);//epsilon is added above
            biases = biases + ((biasMomentumsCorrected.divide(biasCacheRooted)) * learningRate);//epsilon is added above
        }
        else
        {
            weights = weights.add(dWeights.customFunc([=](Type input, int index)
            {
                return input * learningRate; //0.01 is learning rate, change this later
            }));
            biases = biases.add(dBiases.customFunc([=](Type input, int index)
            {
                return input * learningRate;
            }));
        }
        iterations++;
    }

    template<typename Type>
    void Layer<Type>::zeroGradient()
    {
        dWeights = ArrayUtils::Array<Type>(dWeights.getRows(), dWeights.getColumns());
        dBiases = ArrayUtils::Array<Type>(dBiases.getRows(), dBiases.getColumns());
    }

    template <typename Type> void Layer<Type>::setWeights(ArrayUtils::Array<Type>& weightVals)
    {
        weights = std::move(weightVals);
    }

    template <typename Type> void Layer<Type>::setBiases(ArrayUtils::Array<Type>& biasVals)
    {
        biases = std::move(biasVals);
    }

    template <typename Type>
    const ArrayUtils::Array<Type>& Layer<Type>::getWeights() const
    {
        return weights;
    }

    template <typename Type>
    const ArrayUtils::Array<Type>& Layer<Type>::getdWeights() const
    {
        return dWeights;
    }

    template <typename Type>
    const ArrayUtils::Array<Type>& Layer<Type>::getBiases() const
    {
        return biases;
    }

    template <typename Type>
    const ArrayUtils::Array<Type>& Layer<Type>::getdBiases() const
    {
        return dBiases;
    }

    template <typename Type>
    const ArrayUtils::Array<Type>& Layer<Type>::getOutputsPreActive() const
    {
        return outputsPreActive;
    }

    template <typename Type>
    const ArrayUtils::Array<Type>& Layer<Type>::getOutputsPostActive() const
    {
        return outputsPostActive;
    }

    template <typename Type>
    const ArrayUtils::Array<Type>& Layer<Type>::getSelfInputs() const
    {
        return selfInputs;
    }

    template <typename Type>
    std::string Layer<Type>::getActivationFunc() const
    {
        return activationFunc;
    }
}

#endif //LAYERCLASS_H
