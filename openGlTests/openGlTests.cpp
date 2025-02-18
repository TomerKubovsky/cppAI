#include <GLFW/glfw3.h>
#include <iostream>

#define ZOOM_STRENGTH 0.2


struct vector2
{
	float x;
	float y;

	vector2(float xI, float yI)
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

	vector2 operator*(float other) const
	{
		return vector2(x * other, y * other);
	}

	vector2 operator/(float other) const
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
	float x;
	float y;
	float z;

	vector3(float xI, float yI, float zI)
		: x(xI), y(yI), z(zI)
	{}
};

struct extraData
{
	float zoom = 1;
	vector2 offset = vector2(0, 0);
	vector2 screenSize = vector2(0, 0);
	bool mouseLeftHeld = false;
	vector2 mousePositionOld = vector2(0, 0);

	extraData()
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

public:
	agent();

	void render();
};

agent::agent()
{
}

void agent::render()
{
	DrawSquare(pos + offset - size/2, pos + offset + size/2, color);
}

void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset)
{
	void* exDataP = glfwGetWindowUserPointer(window);
	extraData* dataP = (extraData*)exDataP;
	dataP->zoom = std::max(dataP->zoom + yOffset * ZOOM_STRENGTH, 0.1);
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

int main()
{
	GLFWwindow* window;
	glfwInit();

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	const double width = static_cast<double>(mode->width);
	const double height = static_cast<double>(mode->height);

	window = glfwCreateWindow(width, height, "Tomer's Ais Window :)", monitor, NULL);

	glfwMakeContextCurrent(window);

	extraData data;
	data.screenSize = vector2(width, height);
	glfwSetWindowUserPointer(window, &data);

	glfwSetScrollCallback(window, scrollCallBack);
	glfwSetMouseButtonCallback(window, mouseButtonCallBack);
	glfwSetCursorPosCallback(window, mouseMoveCallBack);

	float size = 100;
	agent AI = agent();
	AI.color = vector3(1, 0, 0);
	AI.pos = vector2(0, 0);
	AI.size = vector2((size * data.zoom)/width, (size * data.zoom)/height);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		AI.size = vector2((size * data.zoom)/width, (size * data.zoom)/height);
		AI.offset = data.offset;


		AI.render();

		// if (data.mouseLeftHeld)
		// {
			// DrawSquare(vector2(0, 0), vector2(1, 1), vector3(1, 1, 1));
		// }

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
