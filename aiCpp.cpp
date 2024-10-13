// #include <iostream>
#include <random>
#include "array.hpp"
#include <chrono>
#include <complex.h>


template <typename Type>
class Layer
{
private:
    Array<Type> weights;
    Array<Type> biases;

    Array<Type> dWeights;
    Array<Type> dBiases;

    Array<Type> outputsPreActive;
    Array<Type> selfInputs;

    std::string activationFunc;

public:
    Layer(const int Inputs, const int outputs /*outputs = neurons*/, std::string activationFunc = "none");
    ~Layer();
    void print() const;
    Array<Type> Forwards(const Array<Type>& Inputs);
    Array<Type> ForwardsActivation(const Array<Type>& Inputs) const;
    Array<Type> BackwardsActivation(const Array<Type>& dOutputs) const;
    Array<Type> Backwards(const Array<Type>& dOutputs) const;
    void setWeights(Array<Type>& weightVals);
    void setBiases(Array<Type>& biasVals);
    void updateWeightsAndBiases(Type learningRate);
};




template <typename Type> Layer<Type>::Layer(const int Inputs, const int outputs, std::string activationFunc)
    :
    weights(Array<Type>(outputs, Inputs).customFunc([](Type input, int index)
        {
            return (Type) index;
        })),
    biases(Array<Type>(1, outputs).customFunc([](Type input, int index)
        {
            return (Type) 1;
        })),
    dWeights(Array<Type>(outputs, Inputs)),
    dBiases(Array<Type>(1, outputs)),
    // biases(Array<Type>(1, outputs)), //1 bias for each neuron and there should be a bias for each neuron and outputs = num of neurons
    activationFunc(activationFunc)
    // {{neuron1weight1, neuron1weight2, neuron1weight3},
    //  {neuron2weight1, neuron2weight2, neuron2weight3}} ammount of rows = amount of outputs, amount of columns = amount of inputs
    {}


template <typename Type> Layer<Type>::~Layer() {}


template <typename Type> void Layer<Type>::print() const
{
    std::cout << "weights: ";
    weights.print();
    std::cout << "biases: ";
    biases.print();
}

template <typename Type> Array<Type> Layer<Type>::Forwards(const Array<Type>& Inputs)
{
    selfInputs = Inputs;
    Array<Type> outputs = weights.dotProduct(Inputs);//caclulate inputs * weights

    //add biases to output
    const int outputsCollumns = outputs.GetColumns();
    const int outputRows = outputs.GetRows();

    Type* outputsPreActivePtr = new Type[outputs.GetColumns() * outputs.GetRows()];
    Type* biasesPtr = biases.GetPtr();
    for (int collumnIndex = 0; collumnIndex < outputsCollumns; collumnIndex++)
    {
        for (int rowIndex = 0; rowIndex < outputRows; rowIndex++)
        {
            outputsPreActivePtr[collumnIndex + outputsCollumns * rowIndex] = outputs.GetPtr()[collumnIndex + outputsCollumns * rowIndex] + biasesPtr[collumnIndex];
        }
    }
    outputsPreActive = Array<Type>(outputsPreActivePtr, outputRows, outputsCollumns);
    Array<Type> finalOutput = ForwardsActivation(outputsPreActive);//activation function
    return finalOutput;
}

template <typename Type> Array<Type> Layer<Type>::ForwardsActivation(const Array<Type>& Inputs) const
{
    if (activationFunc == "relu")
    {
        return Inputs.customFunc([](Type input, int index)
        {
            return (input > 0) ? input : 0;
        });
        //makes a lambda function for relu and passess it into inputs custom func
    }else
    {
        return Inputs;
    }
}


/*
    dactiv is the derivative of the inputs to the activbation functions so self.activationBackwards(dOutputs) also you gotta multiply it by dOutputs which is why it is passed into the function
*/
template <typename Type> Array<Type> Layer<Type>::BackwardsActivation(const Array<Type>& dOutputs) const
{
    if (activationFunc == "relu")
    {
        return outputsPreActive.customFunc2Arr(dOutputs, [](Type preActivArrVal, Type dOutputsArrVal, int index)
        {
            // return ((preActivArrVal > 0) ? 1 : 0) * dOutputsArrVal;
            return (preActivArrVal > 0) ? dOutputsArrVal : 0; //these two lines of code do the EXACT SAME THING just bottom one is faster and looks nicer, top one easier to understand tho
        });
    } else
    {
        return dOutputs; //linear activation function derivative is 1 so 1*doutpus so doutputs
    }
}


template <typename Type> Array<Type> Layer<Type>::Backwards(const Array<Type>& dOutputs /*dOutputs = derivative of outputs*/) const
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
    const Array<Type> dActiv = BackwardsActivation(dOutputs);
    // dWeights = selfInputs.Transpose().dotProduct(dActiv.Transpose());

    Type* dWeightsPtr = dWeights.GetPtr();
    const Type* inputPtr = selfInputs.GetPtr();
    const Type* dActivPtr = dActiv.GetPtr();
    Type* dInputsPtr = new Type[selfInputs.GetRows() * selfInputs.GetColumns()];
    const Type* weightsPtr = weights.GetPtr();
    const int inputSize = selfInputs.GetColumns();

    Type* dBiasPtr = dBiases.GetPtr();
    const int neuronAmount = dActiv.GetColumns();

    for (int batchNum = 0; batchNum < selfInputs.GetRows(); batchNum++)
    {
        for (int neuronNum = 0; neuronNum < neuronAmount; neuronNum++)
        {
            for (int inputNum = 0; inputNum < selfInputs.GetColumns(); inputNum++)
            {
                dWeightsPtr[inputNum + neuronNum * inputSize] += inputPtr[inputNum + batchNum * inputSize] * dActivPtr[neuronNum + batchNum * neuronAmount]; //derivative of weight is the input of that batch and weight * derivative of the weights neuron for that batch
                dInputsPtr[inputNum + batchNum * inputSize] += weightsPtr[inputNum + neuronNum * inputSize] * dActivPtr[neuronNum + batchNum * neuronAmount]; //derivative of input for that batch is the weight * derivative fo that weights neuron for that batch
            }

            dBiasPtr[neuronNum] += dActivPtr[neuronNum + batchNum * neuronAmount];
        }
    }

    return  Array<Type>(dInputsPtr, selfInputs.GetRows(), selfInputs.GetColumns());

}



template <typename Type> void Layer<Type>::setWeights(Array<Type>& weightVals)
{
    weights = weightVals;   
}

template <typename Type> void Layer<Type>::setBiases(Array<Type>& biasVals)
{
    biases = biasVals;
}

template <typename Type> void Layer<Type>::updateWeightsAndBiases(Type learningRate)
{
    weights = weights.add(dWeights.multiply(learningRate));
    //add bias later
}


void disruptStack()
{
    int largeArray[10000]; // Larger array
    for (int i = 0; i < 10000; ++i)
        largeArray[i] = i; // Force memory usage
}

int main() {

    // float testLayerData[2][3] = {
    //     {1.0f,1.0f,1.0f},
    //     {2.0f,2.0f,2.0f}
    // };

    // float biasVals[1][2] = {0.0f, 0.0f};

    // Array<float> testLayerBiases = Array<float>(&(biasVals[0][0]),1,2);   

    // Array<float> testLayerWeights = Array<float>(&(testLayerData[0][0]),2,3);



    // testLayer.setWeights(&testLayerWeights);

    // testLayer.setBiases(&testLayerBiases);

    Layer<float> testLayer(3,2, "relu");

    testLayer.print();

    float inputData[2][3] = {{3.0f,2.0f,5.0f},
                                {2.0f, 2.0f, 2.0f}};

    Array<float> Inputs =  Array<float>(&(inputData[0][0]),2,3); //1 batch 3 inputs for that 1 batch

    std::cout << "inputs:";
    Inputs.print();

    Array<float> Outputs = testLayer.Forwards(Inputs);

    std::cout << "outputs:";
    Outputs.print();

    float dOutputsData[2][2] = {{2.0, 2.0},
                                  {1.0, 1.0}};

    Array<float> dOutputs(&(dOutputsData[0][0]), 2, 2);

    Array<float> dInputs = testLayer.Backwards(dOutputs);

    dInputs.print();
}