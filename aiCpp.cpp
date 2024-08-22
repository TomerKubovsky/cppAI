#include <iostream>
#include <random>
#include "array.hpp"

using namespace std;

template <typename Type>
class Layer
{
private:
    Array<Type> weights;
    Type bias;

public:
    Layer(const int inputs, const int outputs /*outputs = neurons*/);
    ~Layer();
    void print() const;
    Array<Type> Forwards(const Array<Type> Inputs) const;
};


template <typename Type> Layer<Type>::Layer(const int inputs, const int outputs)
{
    weights = Array<Type>(outputs, inputs);
    mt19937 gen(random_device());
    uniform_real_distribution<> dis(0.0, 1.0);
    bias = dis(gen);
    // {{neuron1weight1, neuron1weight2, neuron1weight3},
    //  {neuron2weight1, neuron2weight2, neuron2weight3}} ammount of rows = amount of outputs, amount of columns = amount of inputs
}

template <typename Type> Layer<Type>::print()
{
    cout << "weights: " << endl << endl;
    weights.print();
    cout << "bias: " << bias;
}

int main() {

    // Array<float> testArr = Array<float>(3,3);

    // testArr.print();

}