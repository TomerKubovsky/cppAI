#include <iostream>
#include "openGlTests/utilsHeader.h"
#include "mainAi/neurelNetworkClass.h"

void initGLThings()
{
	GLFWwindow* window = nullptr;
	glUtils::initWindow(window, "AI Window");


}

int main()
{
	initGLThings();
	return 0;
}