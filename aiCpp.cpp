#include <iostream>
#include <random>
#include "array.hpp"
#include <chrono>

using namespace std;

template <typename Type>
class Layer
{
private:
    Array<Type>* weights;
    Array<Type>* biases;

    Array<Type>* dWeights;

    string activationFunc;

public:
    Layer(const int Inputs, const int outputs /*outputs = neurons*/, string activationFunc = "none");
    ~Layer();
    void print() const;
    Array<Type> Forwards(const Array<Type>* Inputs) const;
    Array<Type> ForwardsActivation(const Array<Type>* Inputs) const;
    Array<Type> Backwards(const Array<Type>* dOutputs) const;
    void setWeights(Array<Type>* weightVals);
    void setBiases(Array<Type>* biasVals);
};




template <typename Type> Layer<Type>::Layer(const int Inputs, const int outputs, string activationFunc)
{
    weights = Array<Type>::constructArray(outputs, Inputs);
    biases = Array<Type>::constructArray(1, outputs); //1 bias for each neuron and there should be a bias for each neuron and outputs = num of neurons

    this->activationFunc = activationFunc;
    // {{neuron1weight1, neuron1weight2, neuron1weight3},
    //  {neuron2weight1, neuron2weight2, neuron2weight3}} ammount of rows = amount of outputs, amount of columns = amount of inputs
}


template <typename Type> Layer<Type>::~Layer()
{
    delete weights;
    delete biases;
}


template <typename Type> void Layer<Type>::print() const
{
    cout << "weights: ";
    weights->print();
    cout << "biases: ";
    biases->print();
}

template <typename Type> Array<Type> Layer<Type>::Forwards(const Array<Type>* Inputs) const
{
    Type* outputs = weights->dotProduct(*Inputs, Inputs->GetRows());
    Array<Type> outputPreBias = Array<Type>(outputs,Inputs->GetRows()/*amount of rows in inputs = batches = amount of rows in output arr*/,weights->GetRows()/*amount of rows = amount of neurons = amount of collumns in output arr*/);
    Array<Type> outputPreActivationFunc = Array<Type>(outputPreBias.add(biases->GetPtr()), Inputs->GetRows(), weights->GetRows());
    Array<Type> finalOutput = ForwardsActivation(&outputPreActivationFunc);
    delete outputs;
    return finalOutput;
}

template <typename Type> Array<Type> Layer<Type>::ForwardsActivation(const Array<Type>* Inputs) const
{
    if (activationFunc == "relu")
    {
        return Array<Type>(Inputs->customFunc([](Type input){return (input > 0) ? input : 0;}), Inputs->GetRows(), Inputs->GetCollumns()); //makes a lambda function for relu and passess it into inputs custom func
    } else
    {
        return *(Inputs);
    }
}


template <typename Type> Array<Type> Layer<Type>::Backwards(const Array<Type>* dOutputs /*dOutputs = derivative of outputs*/) const
{
    /*
    weights derivatives = inputs since y=wx where x is input and w is weights, and derivative of y=wx with respect to w is x, this also means derivative of inputs is weights since d/dx y=wx is w
    bias deivatives = 1 since the equation is activationFunc(input1*weight1+input2*weight2+bias) and since we are deriviving with respect to bias everything else can be treated as constants
    furthermore with bias derivatives instead of writing it as  d                                         we can write it as   d  below value is 0 bc treated as a constant   d   the one below is 1 since y=x wrt x is 1 and this is y=bias wrt bias =
                                                               ----(input1*weight1+input2*weight2+bias)                       ---- (input1*weight1).......                   ----bias  
                                                               dbias                                                          dbias                                          dbias
    remember all the derivatives must be multiplied by dOutputs due to chain rule 
    */


    //theoretical inputs
    1, 2, 3

    //theoritcal weights, 3 neurons
    1.3, 2.6, 3.8    
    6.3, 9.6, 1.8    
    7.3, 1.1, 3.6   

    //derivatives of weights: add for all batches 
    1, 2, 3
    1, 2, 3
    1, 2, 3

    //derivatives of inputs = 
    1.3 + 6.3 + 7.3, 2.6 + 9.6 + 1.1, 3.8 + 1.8 + 3.6

    Array<Type> dOutputs = Array<Type> 

    for (int Row = 0; Row < weights.GetRows(); Row++)
    {
        for (int Collumn = 0; Collumn < weights.GetCollumns(); Collumn++)
        {

        }
    }

}



template <typename Type> void Layer<Type>::setWeights(Array<Type>* weightVals)
{
    weights = weightVals;   
}

template <typename Type> void Layer<Type>::setBiases(Array<Type>* biasVals)
{
    biases = biasVals;
}



int main() {

    // float testLayerData[2][3] = {
    //     {1.0f,1.0f,1.0f},
    //     {2.0f,2.0f,2.0f}
    // };

    // float biasVals[1][2] = {0.0f, 0.0f};

    // Array<float> testLayerBiases = Array<float>(&(biasVals[0][0]),1,2);   

    // Array<float> testLayerWeights = Array<float>(&(testLayerData[0][0]),2,3);

    Layer<float> testLayer = Layer<float>(3,2, "relu");


    // testLayer.setWeights(&testLayerWeights);

    // testLayer.setBiases(&testLayerBiases);

    testLayer.print();

    float inputData[1][3] = {{3.0f,2.0f,5.0f}};

    Array<float> Inputs = Array<float>(&(inputData[0][0]),1,3); //1 batch 3 inputs for that 1 batch


    Array<float> Outputs = testLayer.Forwards(&Inputs);

    Outputs.print();

}