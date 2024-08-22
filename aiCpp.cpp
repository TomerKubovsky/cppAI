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
    Type bias;

public:
    Layer(const int inputs, const int outputs /*outputs = neurons*/);
    // ~Layer();
    void print() const;
    Array<Type>* Forwards(const Array<Type>* Inputs) const;
};


template <typename Type> Layer<Type>::Layer(const int inputs, const int outputs)
{
    weights = Array<Type>::constructArray(outputs, inputs);
    mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_real_distribution<> dis(0.0, 1.0);
    bias = dis(gen);
    // {{neuron1weight1, neuron1weight2, neuron1weight3},
    //  {neuron2weight1, neuron2weight2, neuron2weight3}} ammount of rows = amount of outputs, amount of columns = amount of inputs
}

template <typename Type> void Layer<Type>::print() const
{
    cout << "weights: ";
    weights->print();
    cout << "bias: " << bias << endl;
}

// template <typename Type> Array<Type>* Layer<Type>::Forwards(Array<Type>* Inputs) const
// {

//     // weights->dotProduct()

// }


int main() {

    Layer<float> testLayer = Layer<float>(2,3);

    testLayer.print();

    Array<float> Inputs = Array<float>(1,2); //1 batch 2 inputs for that 1 batch

    Inputs.print();

}