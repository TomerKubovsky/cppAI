#include <iostream>
#include "array.hpp"

using namespace std;



int main()
{
	/*
	weights:
	1, 3, 4
	2, 6, 9
	
	2 neurons 3 inputs

	inputs = 3, 4, 5

	dOutputs = 2, 1   there are 2 neurons 1 batch so 2 doutputs
	dweights = 3*2, 4*2, 5*2
			   3*1, 4*1, 5*1
	so when weights = 
	a, b, c
	d, e, f

	and inputs = 
	x, y, z
	and dOutputs = 
	h, h, h
	i, i, i
	
	remember that h is the sum of the derivatives of all the batches, same with i

	then dweights=
	x*h, y*h, z*h
	x*i, y*i, z*i

	dotproduct does
	3 inputs 2 outputs
	inputs:
	1, 3, 4
	
	weights dimension 2, 3
	1

	
	for i = 0 and i is smaller than amount of rows of 1,2,3 then do:
		for i2 = 0 and i2 is smaller than the amount of rows in 3, 4, 5 then do
			for i3 = 0 and i3 is smaller than the amount of collumns in 3, 4,5 then do
				addToFinalArray[i]

	1, 2

	5, 9, 7
	
	1,
	2

	1*5, 1*9, 1*7
	2*5, 2*9, 2*7
	*/

	float weightsVals[2][3] = {{1, 3, 4},
						   {2, 6, 9}};
    
    float inputsVals[1][3] = {3,4,5}; 

    float dOutputVals[1][3] = {2,1};

    Array<float> weights = Array<float>(&(weightsVals[0][0]), 2, 3);

    Array<float> inputs = Array<float>(&(inputsVals[0][0]), 1, 3);

    Array<float> dOutputs = Array<float>(&(dOutputVals[0][0]), 1, 2);

    float* outputArr = weights.dotProduct(inputs, 1);

    Array<float> outputs = Array<float>(outputArr, 1, 2);

    // outputs.print();

    float* dWeightsVals = inputs.dotProduct(weights, weights.GetRows());

    Array<float> dWeights = Array<float>(dWeightsVals, 2, 3);

    dWeights.print();
}