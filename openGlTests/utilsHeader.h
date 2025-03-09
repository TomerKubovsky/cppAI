#ifndef openGlUtils_H
#define openGlUtils_H

#include "GLFW/glfw3.h"
//#include <GLFW
#include <iostream>
#include <vector>

#define ZOOM_STRENGTH 0.1
#define decimalType long double
#define ZOOM_BOOL false

namespace glUtils
{
	struct vector2
	{
		decimalType x;
		decimalType y;

		vector2(decimalType xI, decimalType yI)
			: x(xI), y(yI)
		{}

		vector2 operator+(vector2 other) const
		{
			return vector2(x + other.x, y + other.y);
		}

		vector2 operator-(vector2 other) const
		{
			return vector2(x - other.x, y - other.y);
		}

		vector2 operator*(vector2 other) const
		{
			return vector2(x * other.x, y * other.y);
		}
		vector2 operator/(vector2 other) const
		{
			return vector2(x / other.x, y / other.y);
		}

		vector2 operator+(decimalType other) const
		{
			return vector2(x+other, y+other);
		}

		vector2 operator*(decimalType other) const
		{
			return vector2(x * other, y * other);
		}

		vector2 operator/(decimalType other) const
		{
			return vector2(x / other, y / other);
		}

		bool operator==(vector2 other) const
		{
			return x == other.x && y == other.y;
		}

		bool operator!=(vector2 other) const
		{
			return x != other.x || y != other.y;
		}

	};


	struct vector3
	{
		decimalType x;
		decimalType y;
		decimalType z;

		vector3(decimalType xI, decimalType yI, decimalType zI)
			: x(xI), y(yI), z(zI)
		{}
	};


	void DrawSquare(vector2 v1, vector2 v2, vector3 color)
	{
		glColor3f(color.x, color.y, color.z);
		glBegin(GL_QUADS);
		glVertex2f(v1.x, v1.y);
		glVertex2f(v1.x, v2.y);
		glVertex2f(v2.x, v2.y);
		glVertex2f(v2.x, v1.y);
		glEnd();
	}

	class agent
	{
	public:
		vector3 color = vector3(0,0,0);
		vector2 offset = vector2(0,0);
		vector2 pos = vector2(0,0);
		vector2 size = vector2(0,0);
		vector2 offsetExtra = vector2(0,0);

	public:
		agent();

		void render();

		vector2 getSPos(unsigned int screenWidth, unsigned int screenHeight);
		vector2 getSPosWithOff(unsigned int screenWidth, unsigned int screenHeight);
		vector2 getSPosDOff(unsigned int screenWidth, unsigned int screenHeight);
	};

	agent::agent()
	{
	}

	void agent::render()
	{
		DrawSquare(pos + (offset + offsetExtra) - size/2, pos + (offset + offsetExtra) + size/2, color);
		// printf("offset dx %f, offset dy %f, offsetEx %f, offsetEy %f \n", offset.x, offset.y, offsetExtra.x, offsetExtra.y);
	}

	vector2 agent::getSPos(const unsigned int screenWidth, const unsigned int screenHeight)
	{
		return vector2((pos.x + 1) * screenWidth/2, (pos.y + 1) * screenHeight/2);
	}

	vector2 agent::getSPosWithOff(const unsigned int screenWidth, const unsigned int screenHeight)
	{
		return vector2((offsetExtra.x + offset.x + pos.x + 1) * screenWidth/2, (-offsetExtra.y - offset.y + pos.y + 1) * screenHeight/2);
	}

	vector2 agent::getSPosDOff(unsigned int screenWidth, unsigned int screenHeight)
	{
		return vector2((offset.x + pos.x + 1) * screenWidth/2, (offset.y + pos.y + 1) * screenHeight/2);
	}


	struct extraData
	{
		decimalType zoom = 1;
		vector2 offset = vector2(0, 0);
		vector2 screenSize = vector2(0, 0);
		bool mouseLeftHeld = false;
		vector2 mousePositionOld = vector2(0, 0);

		std::vector<agent*> agents;

		extraData()
		{}
	};

	void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset)
	{
		void* exDataP = glfwGetWindowUserPointer(window);
		extraData* dataP = (extraData*)exDataP;

		// dataP->zoom = std::min(std::max(dataP->zoom + yOffset * ZOOM_STRENGTH, 0.6), 5.0);
		decimalType adjY = std::pow(2, yOffset * ZOOM_STRENGTH);
		// decimalType adjY = std::pow(2, yOffset);
		dataP->zoom = dataP->zoom * adjY;

		if (ZOOM_BOOL)
		{
			for (int i = 0; i < dataP->agents.size(); i++)
			{
				agent* obj = dataP->agents[i];
				vector2 objSPos = obj->getSPosWithOff(dataP->screenSize.x, dataP->screenSize.y);
				vector2 mousePos(0, 0);
				double mouseX; double mouseY;
				glfwGetCursorPos(window, &mouseX,&mouseY);
				mousePos.x = mouseX;
				mousePos.y = mouseY;
				vector2 vecFromMouse = objSPos - mousePos;


				vector2 objOffsetExtraAdjusted = vector2(obj->offsetExtra.x * dataP->screenSize.x/2, obj->offsetExtra.y * dataP->screenSize.y/2 * -1); //adjusts -1->1 to 0->screenWidth and height stuff
				vector2 vectorOffsetPreAdj = objOffsetExtraAdjusted + (mousePos + (objSPos - mousePos) * adjY) - objSPos;

				vectorOffsetPreAdj.x = vectorOffsetPreAdj.x / (dataP->screenSize.x/2);
				vectorOffsetPreAdj.y = -vectorOffsetPreAdj.y / (dataP->screenSize.y/2);
				obj->offsetExtra = vectorOffsetPreAdj;
			}
		}

	}

	void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			void* exDataP = glfwGetWindowUserPointer(window);
			extraData* dataP = (extraData*)exDataP;
			if (action == GLFW_PRESS)
			{
				dataP->mouseLeftHeld = true;
			} else if (action == GLFW_RELEASE)
			{
				dataP->mouseLeftHeld = false;
				dataP->mousePositionOld = vector2(0, 0);
			}
		}
	}

	void mouseMoveCallBack(GLFWwindow* window, double xPos, double yPos)
	{
		void* exDataP = glfwGetWindowUserPointer(window);
		extraData* dataP = (extraData*)exDataP;
		if (dataP->mouseLeftHeld)
		{
			vector2 newPos = vector2(xPos, yPos);
			if (dataP->mousePositionOld != vector2(0, 0))
			{
				vector2 offset = (newPos - dataP -> mousePositionOld);
				offset.x /= dataP->screenSize.x;
				offset.y /= -dataP->screenSize.y; //use - datap->screenSize.y bc u need to flip on y bc when you move mouse down its getting greater in pixel pos
				dataP->offset = dataP->offset + offset;
				// std::cout << offset.x << ", " << offset.y << std::endl;
				// std::cout << newPos.x << ", " << newPos.y << std::endl;
			}
			dataP->mousePositionOld = newPos;
		}
	}


	void initWindow (GLFWwindow* window, const char* title)
	{
		glfwInit();

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		const decimalType width = static_cast<decimalType>(mode->width);
		const decimalType height = static_cast<decimalType>(mode->height);

		window = glfwCreateWindow(width, height, title, monitor, NULL);

		glfwMakeContextCurrent(window);

		extraData data;
		data.screenSize = vector2(width, height);
		glfwSetWindowUserPointer(window, &data);

		glfwSetScrollCallback(window, scrollCallBack);
		glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	}
}
#endif
