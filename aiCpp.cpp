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

public:
    Layer(const int inputs, const int outputs /*outputs = neurons*/);
    // ~Layer();
    void print() const;
    Array<Type> Forwards(const Array<Type>* Inputs) const;

    void setWeights(Array<Type>* weightVals);
};


template <typename Type> Layer<Type>::Layer(const int inputs, const int outputs)
{
    weights = Array<Type>::constructArray(outputs, inputs);
    biases = Array<Type>::constructArray(1, outputs); //1 bias for each neuron and there should be a bias for each neuron and outputs = num of neurons
    // {{neuron1weight1, neuron1weight2, neuron1weight3},
    //  {neuron2weight1, neuron2weight2, neuron2weight3}} ammount of rows = amount of outputs, amount of columns = amount of inputs
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

    Array<Type> outputArr = Array<Type>(outputs,Inputs->GetRows()/*amount of rows in inputs = batches = amount of rows in output arr*/,weights->GetRows()/*amount of rows = amount of neurons = amount of collumns in output arr*/);

    Array<Type> finalOutput = Array<Type>(outputArr.add(biases->GetPtr()), Inputs->GetRows(), weights->GetRows());

    delete outputs;

    return finalOutput;
}

template <typename Type> void Layer<Type>::setWeights(Array<Type>* weightVals)
{
    weights = weightVals;   
}


int main() {

    // float testLayerData[2][3] = {
    //     {1.0f,2.0f,0.5f},
    //     {2.0f,2.0f,2.0f}
    // };

    // Array<float> testLayerWeights = Array<float>(&(testLayerData[0][0]),3,2);

    Layer<float> testLayer = Layer<float>(3,2);


    // testLayer.setWeights(&testLayerWeights);

    testLayer.print();

    float inputData[1][3] = {{3.0f,5.0f,2.0f}};

    Array<float> Inputs = Array<float>(&(inputData[0][0]),1,3); //1 batch 3 inputs for that 1 batch


    Array<float> Outputs = testLayer.Forwards(&Inputs);

    Outputs.print();



}