#include <iostream>
#include "openGlTests/utilsHeader.h"
#include "mainAi/neurelNetworkClass.h"



using namespace NeurelNetwork::ArrayUtils;

Array<decimalType> vectorToArray(glUtils::vector2 vec)
{
	decimalType* arrPtr = new decimalType[2];
	arrPtr[0] = vec.x;
	arrPtr[1] = vec.y;

	return Array<decimalType>(arrPtr, 1, 2);
}

glUtils::vector2 arrayToVector(Array<decimalType> arr)
{
	return glUtils::vector2(arr.getPtr()[0], arr.getPtr()[1]);
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

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	const decimalType width = static_cast<decimalType>(mode->width);
	const decimalType height = static_cast<decimalType>(mode->height);
	dataPointer->screenSize = { width, height };
	dataPointer->size = static_cast<decimalType>(0.1);

	mainPreFunc(dataPointer, window);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		mainLoopFunc(dataPointer, window);

		// glUtils::DrawSquare(glUtils::vector2(-0.5,-0.5), glUtils::vector2(0.5,0.5), glUtils::vector3(1.0,0.0,0.0));

		for (glUtils::agent* agent: dataPointer->agents)
		{
			agent->render();
		}

		glfwSwapBuffers(window);
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

	enemyAgent->pos = glUtils::vector2(0.2f, 0.3f);

	glUtils::vector2 agentSize = glUtils::vector2((dataPointer->size * dataPointer->zoom)/(dataPointer->screenSize.x*0.01), (dataPointer->size * dataPointer->zoom)/(dataPointer->screenSize.y*0.01));
	aiAgent->size = agentSize;
	enemyAgent->size = agentSize;

	dataPointer->agents.push_back(aiAgent);
	dataPointer->agents.push_back(enemyAgent);

	const int layers[] = {2, 16, 16, 4};
	dataPointer->neuralnetworks.push_back(&NeurelNetwork::neuralnetwork<decimalType>(layers, 4, "relu", "sigmoid", 0.01, "adam"));
}

void mainFuncLoop(glUtils::extraData* dataP, GLFWwindow* window)
{
	using namespace NeurelNetwork::ArrayUtils;
	constexpr double speed = 0.01;
	glUtils::agent* aiAgent = dataP->agents[0];
	glUtils::agent* enemyAgent = dataP->agents[1];

	glUtils::vector2 vectorToEnemy = enemyAgent->pos - aiAgent->pos;
	const decimalType hyp = std::sqrt(vectorToEnemy.x * vectorToEnemy.x + vectorToEnemy.y  * vectorToEnemy.y); //find hypotenuse of vector
	vectorToEnemy = glUtils::vector2(vectorToEnemy.x/hyp, vectorToEnemy.y/hyp); //normalize vector by dividing by hypotenuse, also did this math in head without help ;)
	enemyAgent->pos = enemyAgent->pos + (vectorToEnemy * dataP->size * speed);

	NeurelNetwork::neuralnetwork<decimalType>* agentNet = dataP->neuralnetworks[0];
	Array<decimalType> inputArr = vectorToArray(vectorToEnemy);
	Array<decimalType> outputArr = agentNet->forwards(inputArr);

	glUtils::vector2 outputVec(0, 0);

	for (int i = 0; i < 4; i++)
	{
		switch (outputArr.getPtr()[i])
		{
			case 0:
				outputVec.x += 1;
			case 1:
				outputVec.y += 1;
			case 2:
				outputVec.x -= 1;
			case 3:
				outputVec.y -= 1;
		}
	}
}

int main()
{
	initGLThings(mainFuncPre, mainFuncLoop);
	return 0;
}