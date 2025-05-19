//
// Created by tomer on 4/05/2025.
//

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "episode.h"
#include "neurelNetworkClass.h"
#include <iostream>

namespace NeurelNetwork
{
namespace EnvironmentClassess
{

	template <typename Type>
	class Environment
	{

	public:
		Environment(neuralnetwork<Type>* aNet, neuralnetwork<Type>* rNet, unsigned int mBatchSize, unsigned int aBatchSize, unsigned int rewardScaler);
		void AddRewards(Type reward);
		void AddEpisode();
		ArrayUtils::Array<Type> Step(ArrayUtils::Array<Type> Inputs);

		void UpdateNets();

	private:
		neuralnetwork<Type>* agentNet;
		neuralnetwork<Type>* rewardNet;
		std::vector<Episode<Type>> episodes;
		const unsigned int actionSize;
		const unsigned int inputSize;
		const unsigned int miniBatchSize;
		const unsigned int amountofMinibatch;
		const unsigned int rewardScaler;
	};

	template <typename Type>
	Environment<Type>::Environment(neuralnetwork<Type>* aNet, neuralnetwork<Type>* rNet, const unsigned int mBatchSize, const unsigned int aBatchSize, const unsigned int rewardScaler):
		agentNet(aNet), rewardNet(rNet), actionSize((aNet->getLayers()[aNet->getNumOfLayers() - 1]).getWeights().getRows()), inputSize((aNet->getLayers()[0]).getWeights().getColumns()), miniBatchSize(mBatchSize), amountofMinibatch(aBatchSize), rewardScaler(rewardScaler)
	//																			subtract 1 here because num of layers is 5 when there are 5 layers but last layer will be at 4 because we start counting at 0
	{
	}


	template <typename Type>
	void Environment<Type>::AddRewards(Type reward)
	{
		episodes.back().AddRewards(reward); //sub 1 bc of explenation 8 lines above but just for episodes arrays
	}

	template <typename Type>
	void Environment<Type>::AddEpisode()
	{
		episodes.push_back(Episode<Type>(actionSize, inputSize));
	}

	template <typename Type>
	ArrayUtils::Array<Type> Environment<Type>::Step(ArrayUtils::Array<Type> Inputs)
	{
		ArrayUtils::Array<Type> outputArr = agentNet->forwards(Inputs);
		ArrayUtils::Array<Type> actionArr = outputArr.customFunc([](Type input, int index)
		{
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 rng(seed);
            std::normal_distribution<> distribution(0.0, 1.0);
			if (distribution(rng) < input)
			{
				return static_cast<Type>(1); //1 means action taken
			} else
			{
				return static_cast<Type>(-1); //-1 means no action
				//TYPE CANT BE unsigned	int !!!!, eg dont set it to unisgned int
			}
		});

		episodes.back().AddValues(actionArr.getPtr(), outputArr.getPtr(), Inputs.getPtr());
		return actionArr;
	}

	template <typename Type>
	void Environment<Type>::UpdateNets()
	{
		for (Episode<Type> episode : episodes)
		{
			episode.CompileRewards(rewardScaler);
		}

		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 rng(seed);
		std::uniform_int_distribution<> episodeDist(0, episodes.size() - 1); //sub 1 bc if theres 1 episode the size = 1 but max index = 0
		for (unsigned int mBatchIndex = 0; mBatchIndex < amountofMinibatch; mBatchIndex++)
		{
			for (unsigned int mBatchStep = 0; mBatchStep < miniBatchSize; mBatchStep++)
			{
				const unsigned int currentEp = episodeDist(rng);
				std::uniform_int_distribution<> stepDist(0, episodes.at(currentEp).GetRewards().size() - 1); //sub 1 same reason as above
				const unsigned int currentStep = stepDist(rng);
				// agentNet->b
			}
		}

	}


}}


#endif //ENVIRONMENT_H
