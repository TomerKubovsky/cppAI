#include "../mainAi/layerClassV2.h"
#include <iostream>

int main()
{
	using namespace NeurelNetwork::ArrayUtils;
	using namespace NeurelNetwork;
	Array<float> inputs(2, 3);

	float* inputPtr = inputs.getHPtr();

	for (int i = 0; i < 6; i++)
	{
		inputPtr[i] = i;
	}

	inputs.updateDevice();

	// inputs.print();

	Layer<float> layer(3, 2);

	Array<float> weights(2, 3);

	float* weightsPtr = weights.getHPtr();

	for (int i = 0; i < 6; i++)
	{
		weightsPtr[i] = i;
	}


	weights.updateDevice();

	// weights.print(); //as

	layer.setWeights(weights);

	Array<float> outputs(std::move(layer.Forwards(inputs)));

	outputs.print();

	layer.getBiases().print();
}