#include <iostream>
#include "openGlTests/utilsHeader.h"
#include "mainAi/neurelNetworkClass.h"
#include <windows.h>
using namespace NeurelNetwork::ArrayUtils;

#define getVal(arr, i) (arr.getPtr())[i]

Array<decimalType> vectorToArray(glUtils::vector2 vec)
{
	decimalType* arrPtr = new decimalType[2];
	arrPtr[0] = vec.x;
	arrPtr[1] = vec.y;

	return Array<decimalType>(arrPtr, 1, 2);
}

glUtils::vector2 arrayToVector(Array<decimalType> arr)
{
	return glUtils::vector2((arr.getPtr())[0], (arr.getPtr())[1]);
}

void trainNet(const unsigned int agentsToTrain, unsigned int time, glUtils::extraData* dataP)
{
	glUtils::vector2* agentsPos = new glUtils::vector2[agentsToTrain];
	NeurelNetwork::neuralnetwork<decimalType>* aiNet = dataP->neuralnetworks[0];
	constexpr bool predictNetBool = false;
	if (predictNetBool == true)
	{
		NeurelNetwork::neuralnetwork<decimalType>* predictNet = dataP->neuralnetworks[1];

		Array<decimalType> predictedRewards;

		for (unsigned int i = 0; i < agentsToTrain; i++)
		{
			// glUtils::vector2 outputVec(0, 0);
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(-0.5, 0.5);
			agentsPos[i] = glUtils::vector2(dis(gen), dis(gen));
		}
		glUtils::vector2 enemyAgentPos = glUtils::vector2(0, 0);
		decimalType* distOlds = new decimalType[agentsToTrain];
		decimalType* inputsPtr = new decimalType[agentsToTrain * 2]; //2 inputs for each agent
		for (unsigned int k = 0; k < time; k++)
		{
			for (unsigned int i = 0; i < agentsToTrain; i++)
			{
				const glUtils::vector2 vecToEnemy = enemyAgentPos - agentsPos[i];
				distOlds[i] = std::sqrt(vecToEnemy.x * vecToEnemy.x + vecToEnemy.y * vecToEnemy.y);
				const glUtils::vector2 normalizedVecToEnemy = vecToEnemy / distOlds[i]; //dist olds[i] is hypotenuse of triangle vectoenemy.x,vectoenemy.y
				inputsPtr[i * 2] = normalizedVecToEnemy.x;
				inputsPtr[i * 2 + 1] = normalizedVecToEnemy.y;
			}
			Array<decimalType> inputsArr(inputsPtr, agentsToTrain, 2);
			predictedRewards = predictNet->forwards(inputsArr);
			inputsArr.autoDeleteMem = false;
			Array<decimalType> outputs = aiNet->forwards(inputsArr);
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.0, 1.0);
			int* actionsArrs = new int[agentsToTrain * 4];
			for (unsigned int agentNum = 0; agentNum < agentsToTrain; agentNum++)
			{
				for (int i = 0; i < 4; i++)
				{
					const double tempVal = dis(gen);
					if (tempVal < (outputs.getPtr())[i + agentNum * 4])
					{
						switch (i)
						{
						case 0:
							agentsPos[agentNum].x += dataP->aiSpeed * dataP->size;
							actionsArrs[i + agentNum * 4] = 1;
							break;
						case 1:
							agentsPos[agentNum].x -= dataP->aiSpeed * dataP->size;
							actionsArrs[i + agentNum * 4] = 1;
							break;
						case 2:
							agentsPos[agentNum].y += dataP->aiSpeed * dataP->size;
							actionsArrs[i + agentNum * 4] = 1;
							break;
						case 3:
							agentsPos[agentNum].y -= dataP->aiSpeed * dataP->size;
							actionsArrs[i + agentNum * 4] = 1;
							break;
						}
					} else
					{
						switch (i)
						{
						case 0:
							actionsArrs[i + agentNum * 4] = -1;
							break;
						case 1:
							actionsArrs[i + agentNum * 4] = -1;
							break;
						case 2:
							actionsArrs[i + agentNum * 4] = -1;
							break;
						case 3:
							actionsArrs[i + agentNum * 4] = -1;
							break;
						}
					}
				}
			}
			decimalType* dOutputsPtr = new decimalType[agentsToTrain * 4];
			decimalType* dOutputsRewardPtr = new decimalType[agentsToTrain];
			for (int agentNum = 0; agentNum < agentsToTrain; agentNum++)
			{
				const glUtils::vector2 vecToEnemy = enemyAgentPos - agentsPos[agentNum];
				constexpr decimalType epsilon = 0.001;
				const decimalType reward = distOlds[agentNum] - std::sqrt(vecToEnemy.x * vecToEnemy.x + vecToEnemy.y * vecToEnemy.y) - epsilon; //epsilon so that when reward = 0 its bad
				dOutputsRewardPtr[agentNum] = ((reward - (predictedRewards.getPtr())[agentNum])*-2)/(agentsToTrain*time);
				// std::cout << reward << " " << predictedRewards.getPtr()[agentNum] << std::endl;
				// dOutputsRewardPtr[agentNum] = (reward)/(agentsToTrain * time);
				for (int i = 0; i < 4; i++)
				{
					dOutputsPtr[i + agentNum * 4] = ((reward - (predictedRewards.getPtr())[agentNum]) * actionsArrs[i + agentNum * 4])/(agentsToTrain * time);
					if (agentNum == 0)
					{
						// std::cout << actionsArrs[i + agentNum * 4] << " " << reward << std::endl;
						// std::cout << dOutputsPtr[i + agentNum * 4] << std::endl;
					}
				}
			}
			Array<decimalType> dOutputs(dOutputsPtr, agentsToTrain, 4);
			Array<decimalType> dOutputsReward(dOutputsRewardPtr, agentsToTrain, 1);
			aiNet->backwards(dOutputs);
			predictNet->backwards(dOutputsReward);
			// std::cout << std::endl << std::endl;
			delete[] actionsArrs;
		}
		delete[] distOlds;
		delete[] agentsPos;
		delete[] inputsPtr; //dont auto delete it when arr gets out of scope bc its reused throughout loops, array is inited in time loop
		aiNet->updateWeightsAndBiases();
		predictNet->updateWeightsAndBiases();
		aiNet->zeroGradient();
		predictNet->zeroGradient();
	}
	else
	{
		for (unsigned int i = 0; i < agentsToTrain; i++)
		{
			// glUtils::vector2 outputVec(0, 0);
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(-0.5, 0.5);
			agentsPos[i] = glUtils::vector2(dis(gen), dis(gen));
		}
		glUtils::vector2 enemyAgentPos = glUtils::vector2(0, 0);
		decimalType* distOlds = new decimalType[agentsToTrain];
		decimalType* inputsPtr = new decimalType[agentsToTrain * 2]; //2 inputs for each agent
		for (unsigned int k = 0; k < time; k++)
		{
			for (unsigned int i = 0; i < agentsToTrain; i++)
			{
				const glUtils::vector2 vecToEnemy = enemyAgentPos - agentsPos[i];
				distOlds[i] = std::sqrt(vecToEnemy.x * vecToEnemy.x + vecToEnemy.y * vecToEnemy.y);
				const glUtils::vector2 normalizedVecToEnemy = vecToEnemy / distOlds[i]; //dist olds[i] is hypotenuse of triangle vectoenemy.x,vectoenemy.y
				inputsPtr[i * 2] = normalizedVecToEnemy.x;
				inputsPtr[i * 2 + 1] = normalizedVecToEnemy.y;
			}
			Array<decimalType> inputsArr(inputsPtr, agentsToTrain, 2);
			inputsArr.autoDeleteMem = false;
			Array<decimalType> outputs = aiNet->forwards(inputsArr);
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.0, 1.0);
			int* actionsArrs = new int[agentsToTrain * 4];
			for (unsigned int agentNum = 0; agentNum < agentsToTrain; agentNum++)
			{
				for (int i = 0; i < 4; i++)
				{
					const double tempVal = dis(gen);
					if (tempVal < (outputs.getPtr())[i + agentNum * 4])
					{
						switch (i)
						{
						case 0:
							agentsPos[agentNum].x += dataP->aiSpeed * dataP->size;
							actionsArrs[i + agentNum * 4] = 1;
							break;
						case 1:
							agentsPos[agentNum].x -= dataP->aiSpeed * dataP->size;
							actionsArrs[i + agentNum * 4] = 1;
							break;
						case 2:
							agentsPos[agentNum].y += dataP->aiSpeed * dataP->size;
							actionsArrs[i + agentNum * 4] = 1;
							break;
						case 3:
							agentsPos[agentNum].y -= dataP->aiSpeed * dataP->size;
							actionsArrs[i + agentNum * 4] = 1;
							break;
						}
					} else
					{
						switch (i)
						{
						case 0:
							actionsArrs[i + agentNum * 4] = -1;
							break;
						case 1:
							actionsArrs[i + agentNum * 4] = -1;
							break;
						case 2:
							actionsArrs[i + agentNum * 4] = -1;
							break;
						case 3:
							actionsArrs[i + agentNum * 4] = -1;
							break;
						}
					}
				}
			}
			decimalType* dOutputsPtr = new decimalType[agentsToTrain * 4];
			for (int agentNum = 0; agentNum < agentsToTrain; agentNum++)
			{
				// if (distOlds[agentNum] >= 0.1)
				if (distOlds[agentNum] >= 0.01)
				{
					const glUtils::vector2 vecToEnemy = enemyAgentPos - agentsPos[agentNum];
					constexpr decimalType epsilon = 0.001;
					const decimalType reward = distOlds[agentNum] - std::sqrt(vecToEnemy.x * vecToEnemy.x + vecToEnemy.y * vecToEnemy.y) - epsilon; //epsilon so that when reward = 0 its bad
					// const decimalType reward = distOlds[agentNum] - std::sqrt(vecToEnemy.x * vecToEnemy.x + vecToEnemy.y * vecToEnemy.y); //epsilon so that when reward = 0 its bad
					dataP->averageReward += reward;
					dataP->rewardCounter++;
					for (int i = 0; i < 4; i++)
					{
						dOutputsPtr[i + agentNum * 4] = (reward * actionsArrs[i + agentNum * 4])/(agentsToTrain * time);
					}
				} else
				{
					for (int i = 0; i < 4; i++)
					{
						dOutputsPtr[i + agentNum * 4] = 0;
					}
				}
			}
			Array<decimalType> dOutputs(dOutputsPtr, agentsToTrain, 4);
			aiNet->backwards(dOutputs);
			// std::cout << std::endl << std::endl;
			delete[] actionsArrs;
		}
		delete[] distOlds;
		delete[] agentsPos;
		delete[] inputsPtr; //dont auto delete it when arr gets out of scope bc its reused throughout loops, array is inited in time loop
		aiNet->updateWeightsAndBiases();
		aiNet->zeroGradient();
	}
}

void initGLThings(void (*mainPreFunc)(glUtils::extraData*, GLFWwindow*), void (*mainLoopFunc)(glUtils::extraData*, GLFWwindow*))
{
	GLFWwindow* window = nullptr;
	glUtils::initWindow(&window, "AI Window");
	glfwMakeContextCurrent(window);

	glUtils::extraData* dataPointer = new glUtils::extraData();

	glfwSetWindowUserPointer(window, dataPointer);

	glfwSetMouseButtonCallback(window, glUtils::mouseButtonCallBack);
	glfwSetCursorPosCallback(window, glUtils::mouseMoveCallBack);
	glfwSetScrollCallback(window, glUtils::scrollCallBack);
	glfwSetKeyCallback(window, glUtils::keyCallBack);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	const decimalType width = static_cast<decimalType>(mode->width);
	const decimalType height = static_cast<decimalType>(mode->height);
	dataPointer->screenSize = { width, height };
	dataPointer->size = static_cast<decimalType>(0.5);
	dataPointer->enemySpeed = static_cast<decimalType>(0.001);
	dataPointer->aiSpeed = dataPointer->enemySpeed * 2;
	dataPointer->count = 0;

	mainPreFunc(dataPointer, window);

	unsigned int countTrue = 0;
	constexpr unsigned int times = 50000;
	bool notDone = true;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		if (dataPointer->pause != true)
		{
			if (dataPointer->resetPos == true)
			{
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<> dis(-0.5,0.5);
				((dataPointer->agents)[0])->pos = glUtils::vector2(dis(gen), dis(gen));
				dataPointer->count = 0;
				dataPointer->resetPos = false;
			}
			if (dataPointer->trainAi)
				trainNet(20,400 , dataPointer);
			mainLoopFunc(dataPointer, window);

			if (countTrue % 100 == 0)
			{
				std::cout << countTrue << " " << (dataPointer->averageReward) / (dataPointer->rewardCounter) << std::endl;
			}
			if (countTrue == times && notDone)
			{
				std::cout << countTrue << " " << (dataPointer->averageReward) / (dataPointer->rewardCounter) << std::endl;
				// glfwDestroyWindow(window);
				dataPointer->trainAi = false;
				dataPointer->pause = true;
				notDone = false;
			}
			countTrue++;

		}
		for (glUtils::agent* agent: dataPointer->agents)
		{
			agent->render();
		}
		glfwSwapBuffers(window);

	}

	for (auto network : dataPointer->neuralnetworks)
	{
		delete network;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

void mainFuncPre(glUtils::extraData* dataPointer, GLFWwindow* window)
{
	glfwMakeContextCurrent(window);
	glUtils::agent* aiAgent = new glUtils::agent();
	glUtils::agent* enemyAgent = new glUtils::agent();

	aiAgent->color = glUtils::vector3(1.0f, 0.0f, 0.0f);
	enemyAgent->color = glUtils::vector3(0.0f, 1.0f, 0.0f);

	aiAgent->pos = glUtils::vector2(-0.25f, -0.25f);
	enemyAgent->pos = glUtils::vector2(0.0f, 0.0f);

	glUtils::vector2 agentSize = glUtils::vector2((dataPointer->size * dataPointer->zoom)/(dataPointer->screenSize.x*0.01), (dataPointer->size * dataPointer->zoom)/(dataPointer->screenSize.y*0.01));
	aiAgent->size = agentSize;
	enemyAgent->size = agentSize;

	dataPointer->agents.push_back(aiAgent);
	dataPointer->agents.push_back(enemyAgent);

	const int layers[] = {2, 16, 16, 4};
	const int layersRegNet[] = {2, 16, 16, 1};
	dataPointer->neuralnetworks.push_back(new NeurelNetwork::neuralnetwork<decimalType>(layers, 4, "leakyRelu", "sigmoid", 0.0003, "adam"));
	dataPointer->neuralnetworks.push_back(new NeurelNetwork::neuralnetwork<decimalType>(layersRegNet, 4, "relu", "none", -0.0001, "adam"));
}

void mainFuncLoop(glUtils::extraData* dataP, GLFWwindow* window)
{
	// if (dataP->count >= 1)
	{
		using namespace NeurelNetwork::ArrayUtils;
		glUtils::agent* aiAgent = dataP->agents[0];
		glUtils::agent* enemyAgent = dataP->agents[1];

		glUtils::vector2 aiOrgPos = aiAgent->pos;

		glUtils::vector2 vectorToEnemy = enemyAgent->pos - aiAgent->pos;
		const decimalType hyp = std::sqrt(vectorToEnemy.x * vectorToEnemy.x + vectorToEnemy.y  * vectorToEnemy.y); //find hypotenuse of vector
		vectorToEnemy = glUtils::vector2(vectorToEnemy.x/hyp, vectorToEnemy.y/hyp); //normalize vector by dividing by hypotenuse, also did this math in head without help ;)
		// enemyAgent->pos = enemyAgent->pos + (vectorToEnemy * dataP->size * dataP->enemySpeed);

		NeurelNetwork::neuralnetwork<decimalType>* agentNet = (dataP->neuralnetworks)[0];
		Array<decimalType> inputArr = vectorToArray(vectorToEnemy);
		Array<decimalType> outputArr = agentNet->forwards(inputArr);

		vectorToEnemy = enemyAgent->pos - aiAgent->pos;
		const decimalType oldDist = std::sqrt(vectorToEnemy.x * vectorToEnemy.x + vectorToEnemy.y  * vectorToEnemy.y); //find hypotenuse of vector

		glUtils::vector2 outputVec(0, 0);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0.0,1.0);

		decimalType* actionsArr = new decimalType[4]();

		for (int i = 0; i < 4; i++)
		{
			const double tempVal = dis(gen);
			if (tempVal < (outputArr.getPtr())[i])
			{
				switch (i)
				{
					case 0:
						outputVec.x += 1;
						actionsArr[0] = 1;
						break;
					case 1:
						outputVec.x -= 1;
						actionsArr[1] = 1;
						break;
					case 2:
						outputVec.y += 1;
						actionsArr[2] = 1;
						break;
					case 3:
						outputVec.y -= 1;
						actionsArr[3] = 1;
						break;
				}
			} else
			{
				switch (i)
				{
				case 0:
					actionsArr[0] = -1;
					break;
				case 1:
					actionsArr[1] = -1;
					break;
				case 2:
					actionsArr[2] = -1;
					break;
				case 3:
					actionsArr[3] = -1;
					break;
				}
			}
		}
		aiAgent->pos = aiAgent->pos + outputVec * dataP->size * dataP->aiSpeed;
/*
		NeurelNetwork::neuralnetwork<decimalType>* predNet = dataP->neuralnetworks[1];
		Array<decimalType> predOutputs = predNet->forwards(inputArr);
		vectorToEnemy = enemyAgent->pos - aiAgent->pos;
		*/
		const decimalType newDist = std::sqrt(vectorToEnemy.x * vectorToEnemy.x + vectorToEnemy.y  * vectorToEnemy.y); //find hypotenuse of vector

		const decimalType distanceDiffrence = oldDist - newDist;
/*
		decimalType* dOutputsPtr = new decimalType[4];
		decimalType* dRewardsPtr = new decimalType[1];
		dRewardsPtr[0] = -2 * (distanceDiffrence - (predOutputs.getPtr())[0]);
		for (int i = 0; i < 4; i++)
		{
			dOutputsPtr[i] = (distanceDiffrence - (predOutputs.getPtr())[0]) * actionsArr[i];
		}
		// dOutputsPtr[3] = -1;
		Array<decimalType> dOutputs(dOutputsPtr, 1, 4);
		Array<decimalType> dRewards(dRewardsPtr, 1, 1);

		*/
		dataP->count++;
		if (dataP->count >= 1000 || newDist <= ((dataP->size * dataP->zoom)/(dataP->screenSize.x*0.01))/2)
		{
			std::uniform_real_distribution<> dis(-0.5,0.5);
			aiAgent->pos = glUtils::vector2(dis(gen), dis(gen));
			enemyAgent->pos =  glUtils::vector2(0, 0);
			dataP->count = 0;
		}
		if (dataP->count % 5 == 0)
		{
			// std::cout << oldDist << std::endl;
			// outputArr.print();
			// std::cout << std::endl << distanceDiffrence;
			// (((dataP->neuralnetworks)[1])->forwards(inputArr)).print();
			// std::cout << std::endl;
		}
		/*
		std::cout << std::endl;
		std::cout << distanceDiffrence;
		outputArr.print();

		// std::cout << std::endl << outputArr.getPtr()[0] << " " << outputArr.getPtr()[1] << std::endl;

		for (int i = 0; i < 4; i++)
		{
			std::cout << actionsArr[i] << " ";
		}
		std::cout << std::endl;
		for (int i = 0; i < 4; i++)
		{
			std::cout << dOutputsPtr[i] << " ";
		}
		std::cout << std::endl;

		agentNet->backwards(dOutputs);
		agentNet->updateWeightsAndBiases();
		agentNet->zeroGradient();
		predNet->backwards(dRewards);
		predNet->updateWeightsAndBiases();
		predNet->zeroGradient();

		// aiAgent->pos = aiOrgPos;
		*/
	}
}

int main()
{
	initGLThings(mainFuncPre, mainFuncLoop);

	// const int layers[4] = {2, 16, 16, 1};
	// NeurelNetwork::neuralnetwork<decimalType> testNet(layers, 4, "relu", "none", -0.001, "none");
	// decimalType* inputPtr = new decimalType[2];
	// for (int i = 0; i < 2; i++)
	// {
	// 	inputPtr[i] = 0.123;
	// }
	// Array<decimalType> inputArr(inputPtr, 1, 2);
	// constexpr decimalType corOut = -0.4;
	// decimalType* dOutputsPtr = new decimalType[1];
	//
	// for (int i = 0; i < 2000; i++)
	// {
	// 	Array<decimalType> outputArr = testNet.forwards(inputArr);
	// 	outputArr.print();
	// 	std::cout << std::endl;
	// 	dOutputsPtr[0] = (-2*(corOut - (outputArr.getPtr())[0])) / (1*1);
	// 	Array<decimalType> dOutputs(dOutputsPtr, 1, 1);
	// 	dOutputs.autoDeleteMem = false;
	// 	testNet.backwards(dOutputs);
	// 	testNet.updateWeightsAndBiases();
	// 	testNet.zeroGradient();
	// }
	// delete[] dOutputsPtr;
	//
	return 0;
}