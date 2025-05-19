//
// Created by tomer on 15/01/2025.
//

// #include "arrayV2.h"

#include "neurelNetworkClass.h"
#include "environment.h"

#define valType double

int main()
{
	using namespace NeurelNetwork;
	using namespace EnvironmentClassess;

	int* layers = new int[3] {1, 4, 1};

	neuralnetwork<valType> actionNet(layers, 3, "relu", "sigmoid", 0.01, "adam");
	neuralnetwork<valType> predictorNet(layers, 3, "relu", "none", 0.01, "adam");
	Environment<valType> env(&actionNet, &predictorNet, 100, 100, 0.9);

	env.AddEpisode();

	valType* inputsPtr = new valType;
	inputsPtr[0] = 1;
	ArrayUtils::Array<valType> inpArr(inputsPtr, 1, 1);
	env.Step(inpArr);
	valType* actionSet = new valType[1]();

	env.AddRewards(1);
	env.Step(inpArr);
	env.AddRewards(1);
	env.Step(inpArr);
	env.AddRewards(1);
	env.Step(inpArr);
	env.AddRewards(1);

	env.UpdateNets();
}
