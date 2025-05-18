#ifndef episode_h
#define episode_h
// #include "neurelNetworkClass.h"
#include <iostream>

namespace NeurelNetwork
{
namespace EnvironmentClassess
{
	template <typename Type>
	class Episode
	{

	public:
		Episode(Type actionSize, Type inputSize);
		void AddValues(Type* actionSet, Type* outputSet, Type* inputSet);
		void AddRewards(Type reward);

		void CompileRewards(Type rewardScaler);

	private:
		std::vector<Type> rewards;
		std::vector<Type> adjustedRewards;
		std::vector<Type> actions;
		std::vector<Type> outputs;
		std::vector<Type> inputs;
		const int actionSize;
		const int inputSize;
	};

	template <typename Type>
	Episode<Type>::Episode(Type actionSize, Type inputSize):
		actionSize(actionSize), inputSize(inputSize)
	{
	}

	template <typename Type>
	void Episode<Type>::AddValues(Type* actionSet, Type* outputSet, Type* inputSet)
	{
		actions.insert(actions.end(), actionSet, actionSet+actionSize - 1); //we subtract 1 from the action set bc basically if there are x actions then action set will be pointing to the first action and action size = x, actionset+action size will give u one more than the last action because action size accounts for the first action meaning ur counting it twice
		outputs.insert(outputs.end(), outputSet, outputSet+actionSize - 1);
		inputs.insert(inputs.end(), inputSet, inputSet+inputSize - 1);
	}
	template <typename Type>
	void Episode<Type>::AddRewards(Type reward)
	{
		rewards.push_back(reward);
	}
	template <typename Type>
	void Episode<Type>::CompileRewards(const Type rewardScaler)
	{
		const int rewardSize = rewards.size();
		adjustedRewards.reserve(rewardSize);
		adjustedRewards[rewardSize] = rewards[rewardSize - 1];
		for (int i = rewardSize - 1; i >= 0; i--)
		{
			adjustedRewards[i] = rewards[i] + adjustedRewards[i + 1] * rewardScaler;
		}
	}
}
}

#endif