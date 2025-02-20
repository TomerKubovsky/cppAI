#ifndef arrayUtils_H
#define arrayUtils_H
#include <functional>
#include <iomanip>
#include <iostream>
namespace NeurelNetwork
{
	namespace ArrayUtils
	{
template <typename Type>
class Array
{
	unsigned int R;
	unsigned int C;
	Type* ptr;

public:
	Array();//defult constructor shouldnt be used UNLESS you assign the array immedietly after, use with EXTREME CAUTION
	Array(Type* arrPtr, unsigned int Rows, unsigned int Columns);
	Array(Type& arrPtr);
	Array(unsigned int Rows, unsigned int Columns);

	Array(Array<Type>&& otherArr/*other array is a temporary object that isa getting deleted*/) noexcept;	//move constructor
	Array(const Array<Type>& otherArr);//copy constructor
	Array& operator=(const Array<Type>& arr);
	Array& operator=(Array<Type>&& arr); //move assignment operator
	static Array<Type>* constructArray(unsigned int Rows, unsigned int Columns);
	Array deepCopy() const;

	void print() const;
	void nullify();

	Array dotProduct(const Array<Type>& inputArray) const; //takes an array of inputs and multiplys it with its own weights, the values in THIS array is the weights
	Array operator+(const Array<Type>& arr);
	Array add(const Array<Type>& inputs) const;
	Array operator-(const Array<Type>& arr);
	Array subtract(const Array<Type>& inputs) const;
	Array multiply(const Array<Type>& inputs) const;
	Array operator*(const Array<Type>& arr);
	Array operator*(const Type val);
	Array divide(Array<Type>& inputs) const;
	Array Transpose() const; //returns transposed array
	Array customFunc(std::function<Type(Type, int)> func) const;
	Array customFunc2Arr(const Array<Type>& arr2, std::function<Type(Type, Type, int)> func) const;

	~Array()
	{
		delete[] ptr;
	}

	int getRows() const;
	int getColumns() const;
	Type* getPtr() const;



};

template <typename Type> Array<Type>::Array():
	R(0),
	C(0),
	ptr(nullptr)
{
}


template <typename Type>
Array<Type>::Array(Type* arrPtr,unsigned const int Rows,unsigned const int Columns):
	R(Rows),
	C(Columns),
	ptr(arrPtr)
{
}

template <typename Type>
Array<Type>::Array(Type& arrPtr)
{
	*this = arrPtr;
}

template <typename Type>
Array<Type>::Array(unsigned const int Rows, unsigned const int Columns):
	R(Rows),
	C(Columns),
	ptr(new Type[R*C]())//() makes it intiliaze at 0
{
	// mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
	// uniform_real_distribution<> dis(0.0, 1.0);
	// for (int index = 0; index < R * C; index++)
	// {
	// 	// ptr[index] = dis(gen);
	// 	ptr[index] = 0.0f;
	// }

}

template <typename Type>
Array<Type>::Array(Array<Type>&& otherArr) noexcept:
	R(otherArr.R),
	C(otherArr.C),
	ptr(otherArr.ptr)
{
	// std::cout << "move cconsturctor good" << std::endl;
	otherArr.ptr = nullptr;
	otherArr.R = 0;
	otherArr.C = 0;
}

template <typename Type>
Array<Type>::Array(const Array<Type>& otherArr)//copy constuctor
	: R(otherArr.R), C(otherArr.C), ptr(otherArr.ptr)
{
}

template <typename Type>
Array<Type>& Array<Type>::operator=(const Array<Type>& arr)
{

	// std::cout << "copy operator used" << std::endl;
	// if (ptr != arr.ptr)
	// {
	// 	delete[] ptr;
	// }
	C = arr.C;
	R = arr.R;
	ptr = arr.ptr;
	return *this;
}

template<typename Type>
Array<Type>& Array<Type>::operator=(Array<Type> &&arr)
{
	// std::cout << "move operator used, good" << std::endl;
	delete[] ptr;
	ptr = arr.ptr;
	R = arr.R;
	C = arr.C;

	arr.ptr = nullptr;
	return *this;
}

template <typename Type>
Array<Type>* Array<Type>::constructArray(unsigned const int Rows, unsigned const int Columns)
{
	return new Array<Type>(Rows, Columns);
}

template<typename Type>
Array<Type> Array<Type>::deepCopy() const

{
	Type* newArrPtr = new Type[R*C]();
	for (int index = 0; index < R * C; index++)
	{
		newArrPtr[index] = ptr[index];
	}
	// return std::move(Array<Type>(newArrPtr, R, C));
	return Array<Type>(newArrPtr, R, C);
}


template <typename Type>
void Array<Type>::print() const
{

	for (int index = 0; index < R * C; index++)
	{
		if (index % C == 0)
		{
			std::cout << std::endl;
		} else
		{
			std::cout << ' ';
		}
		std::cout << ptr[index];
	}
	std::cout << std::endl;
}

template<typename Type>
void Array<Type>::nullify()
{
	ptr = nullptr;
	R = 0;
	C = 0;
}

template <typename Type>
Array<Type> Array<Type>::dotProduct(const Array<Type>& inputArray) const
{
	//amount of rows = amount of neurons so the amount of rows = the amount of columns in the final array, the amount of rows in the input array = the amount of batches, so the amount of rows in input array = amount of rows in the output array

	if (inputArray.C != C) {throw std::invalid_argument("input array and self array columns do not match in dot product");}

	const int& batches = inputArray.R;


	Type* batchesOutputs = new Type[batches * R]();



	for (int batchNum /*aka row num for input array*/ = 0; batchNum < batches; batchNum++)
	{
		for (int weightListNum = 0; weightListNum < R; weightListNum++)
		{
			for (int weightNum = 0; weightNum < C; weightNum++)
			{
				batchesOutputs[batchNum * R + weightListNum] += ptr[weightListNum * C + weightNum] * inputArray.ptr[batchNum * C + weightNum];
			}
		}

	}
	/*
	this code is built to be like weights.DotProduct(inputs) where weights look like:
	neuron1Weight1, neuron1Weight2
	neuron2Weight1, neuron2Weight2
	and inputs:
	input1batch1, input2batch1
	input1batch2, input2batch2

	columns have to match, rows dont
	*/
	return Array<Type>(batchesOutputs, inputArray.R, R);
}

template<typename Type>
Array<Type> Array<Type>::operator+(const Array<Type> &arr)
{
	// Array<Type>& addedArrRef = add(arr);
	// return Array<Type>(addedArrRef.ptr, addedArrRef.R, addedArrRef.C);
	return add(arr);
}


template <typename Type>
Array<Type> Array<Type>::add(const Array<Type>& inputs) const
{
	if (inputs.R != R) {throw std::invalid_argument("inputs rows is not equal to self rows in add arr");}
	if (inputs.C != C) {throw std::invalid_argument("inputs columns is not equal to self columns in add arr");}

	Type* outputs = new Type[R*C];
	Type* inputPtr = inputs.ptr;
	for (int index = 0; index < R*C; index++)
	{
		*(outputs + index) = *(ptr + index) + *(inputPtr + index);
	}

	return Array<Type>(outputs, R, C);
}

template<typename Type>
Array<Type> Array<Type>::operator-(const Array<Type> &arr)
{
	Array<Type>& subbedArrRef = subtract(arr);
	return Array<Type>(subbedArrRef.ptr, subbedArrRef.R, subbedArrRef.C);
}

template<typename Type>
Array<Type> Array<Type>::subtract(const Array<Type>& inputs) const
{
	if (inputs.R != R) {throw std::invalid_argument("inputs rows is not equal to self rows in subtract arr");}
	if (inputs.C != C) {throw std::invalid_argument("inputs columns is not equal to self columns in subtrct arr");}

	Type* outputs = new Type[R*C];
	Type* inputPtr = inputs.ptr;
	for (int index = 0; index < R*C; index++)
	{
		*(outputs + index) = *(ptr + index) - *(inputPtr + index);
	}

	return Array<Type>(outputs, R, C);
}

template <typename Type>
Array<Type> Array<Type>::multiply(const Array<Type>& inputs) const
{
	if (inputs.R != R) {throw std::invalid_argument("inputs rows is not equal to self rows in mult arr");}
	if (inputs.C != C) {throw std::invalid_argument("inputs columns is not equal to self columns in mult arr");}

	Type* outputs = new Type[R*C];
	Type* inputPtr = inputs.ptr;
	for (int index = 0; index < R*C; index++)
	{
		*(outputs + index) = *(ptr + index) * *(inputPtr + index);
	}

	return Array<Type>(outputs, R, C);
}

template<typename Type>
Array<Type> Array<Type>::operator*(const Array<Type> &arr)
{
	return multiply(arr);
}

template <typename Type>
Array<Type> Array<Type>::operator*(const Type val)
{
	return customFunc([=](Type thisArrVal, int index)
	{
		return val * thisArrVal;
	});
}

template <typename Type>
Array<Type> Array<Type>::divide(Array<Type>& inputs) const
{
	if (inputs.R != R) {throw std::invalid_argument("inputs rows is not equal to self rows in divide arr");}
	if (inputs.C != C) {throw std::invalid_argument("inputs columns is not equal to self columns in divide arr");}

	Type* outputs = new Type[R*C];
	Type* inputPtr = inputs.ptr;
	for (int index = 0; index < R*C; index++)
	{
		*(outputs + index) = *(ptr + index) / *(inputPtr + index);
	}

	return Array<Type>(outputs, R, C);
}


template <typename Type>
Array<Type> Array<Type>::Transpose() const
{
	Type* tempPtr = new Type[R*C];
	for (int row = 0; row < R; row++)
	{
		for (int collumn = 0; collumn < C; collumn++)
		{
			tempPtr[collumn * R + row] = ptr[row * C + collumn];
		}
	}

	return Array<Type>(tempPtr, C, R); //rows and columns are reversed after transpose
}

template <typename Type>
Array<Type> Array<Type>::customFunc(std::function<Type(Type, int)> func) const
{
	Type* tempPtr = new Type[R*C];
	for (int index = 0; index < R*C; index++)
	{
		*(tempPtr + index) = func(*(ptr + index), index);
	}
	return Array<Type>(tempPtr, R, C);
}


template <typename Type>
Array<Type> Array<Type>::customFunc2Arr(const Array<Type>& arr2, std::function<Type(Type, Type, int)> func) const
{
	if (arr2.R != R) {std::cerr << "arr2 rows do not equal arr1 rows in customFunc2Arr :(" << std::endl;}
	if (arr2.C != C) {std::cerr << "arr2 columns do not equal arr1 columns in customFunc2Arr :(" << std::endl;}

	Type* arr2Ptr = arr2.ptr; //pointer doesnt need to be delted as arr2 is responsible for controling that memory and has a destructor

	Type* tempPtr = new Type[R*C];
	for (int index = 0; index < R*C; index++)
	{
		*(tempPtr + index) = func(*(ptr+index), *(arr2Ptr + index), index);
	}
	return Array<Type>(tempPtr, R, C);
}

template <typename Type>
int Array<Type>::getRows() const
{
	return R;
}

template <typename Type>
int Array<Type>::getColumns() const
{
	return C;
}

template <typename Type>
Type* Array<Type>::getPtr() const
{
	return ptr;
}
	}
}



#endif //end of arrayUtils_H





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
                return static_cast<Type>(distribution(rng) * 0.002);
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
                return static_cast<Type>(distribution(rng) * 0.002);
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
