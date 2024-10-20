#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <sstream>

#include "textures/allTex1.ppm"
#define DR 0.0174533
#define PI 3.14159265
#define PI2 PI/2
#define PI3 3*PI/2
// set most things here
int SCR_WIDTH = 1024;
int SCR_HEIGHT = 512;

int textureResolution = 32;

const double fpsLimit = 1.0 / 60.0;

float playerX,playerY, playerDirectionX, playerDirectionY;
float playerAngle = 1.0f;
float playerMoveSpeed = 1.0f;
float playerTurnSpeed = 0.05f;

// create funny raycaster map, width and height are the dimenions of the square, size is area
int mapGridWidth = 8;
int mapGridHeight = 8;
int mapGridSize = mapGridWidth * mapGridHeight;
int wallMap[] = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 0, 0, 1,
    1, 2, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 0, 0, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
};

int floorMap[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

int All_Textures[] = {
 //Checkerboard (test texture)
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,0,0,0,0,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,0,0,0,0,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,

 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1, 0,0,0,0,0,0,0,0,
 //Brick
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0, 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 //Window
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,

 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,

 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,

 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1,
 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,
 //Door
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,

 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,0,0,0,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,0,0,0,1,0,0,0,
 0,0,0,1,1,1,1,1, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 1,1,1,1,1,0,0,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,1,0,1, 1,0,1,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,1,1,1,1,0,1, 1,0,1,1,1,1,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,

 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,1, 1,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
 0,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,0,
};

void processInput (GLFWwindow* window);
// cycles through all of the values and decides wether to draw a square or not
void drawMap2D() {
    for (int y = 0; y < mapGridHeight; y++) {
        for (int x = 0; x < mapGridWidth; x++) {
            // set an offset to make sure squares are next to eachother (by offsetting it all by the dimensions of the squares)
            int xOffset = x * mapGridSize;
            int yOffset = y * mapGridSize;
            // its y * map width + x to ensure that it does more than just the top row
            if (wallMap[y*mapGridWidth+x] > 0) {
                glColor3f(1.0f, 1.0f, 1.0f);
            } else {
                glColor3f(0.0f, 0.0f, 0.0f);
            }
            // draw the map and offset each line by 1 pixel to make a grid
            glBegin(GL_QUADS);
            glVertex2i(xOffset+1, yOffset+1);
            glVertex2i(xOffset+1, yOffset + mapGridSize-1);
            glVertex2i(xOffset-1 + mapGridSize, yOffset + mapGridSize-1);
            glVertex2i(xOffset-1 + mapGridSize, yOffset+1);
            glEnd();
        }
    }
}

void drawPlayer2D() {

    glColor3f(1.0f, 0.0f, 0.0f);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2f(playerX,playerY);
    glEnd();

    // draw a line to show which way the player is facing in 2D view
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2i(playerX,playerY);
    glVertex2i(playerX+playerDirectionX*20,playerY+playerDirectionY*20);
    glEnd();
}

float pythag(float ax, float ay, float bx, float by) {
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void drawRays() {
    // draw smth that looks like a sky
    glColor3f(0, 1, 1);
    glBegin(GL_QUADS);
    glVertex2i(526, 0);
    glVertex2i(1006, 0);
    glVertex2i(1006, 160);
    glVertex2i(526, 160);
    glEnd();
    // draw smth to look like the group
    glColor3f(0, 0, 1);
    glBegin(GL_QUADS);
    glVertex2i(526, 160);
    glVertex2i(1006, 160);
    glVertex2i(1006, 320);
    glVertex2i(526, 320);
    glEnd();
    // set ray angle to player angle + an offset
    float fov = 60; float rayResoultion = 1.0;
    float rayAngle = playerAngle + fov * DR / 2;
    if (rayAngle < 0)    { rayAngle += 2*PI; }
    if (rayAngle > 2*PI) { rayAngle -= 2*PI; }

    // checks to make sure the ray doesnt become values the dumb trig ratios cant work with
    if (rayAngle < 0)    { rayAngle += 2 * PI; }
    if (rayAngle > 2*PI) { rayAngle -= 2*PI;   }

    float rayY, rayX, xOffset, yOffset;
    float distance, shade;
    int  mapX, mapY, mapTile, depthOfField, r;
    int mapTileValueH, mapTileValueV;


    // for loop to draw rays
    for (r = 0; r<fov/rayResoultion; r++) {

        // --------------------------- horizontal ray check -----------------------------
        float aTan = -1/tan(rayAngle);
        depthOfField = 0;
        float horiDistance= 100000000000000000, horiX=playerX, horiY = playerY;

        if (rayAngle > PI) {
            rayY = (((int)playerY>>6)<<6)-0.001;
            rayX = (playerY-rayY) * aTan+playerX;
            yOffset =-64;
            xOffset = -yOffset*aTan;
        }
        if (rayAngle < PI) {
            rayY = (((int)playerY>>6)<<6)+64;
            rayX = (playerY-rayY) * aTan+playerX;
            yOffset = 64;
            xOffset = -yOffset*aTan;
        }

        if (rayAngle == 0 || rayAngle == PI) { rayX = playerX; rayY = playerY; depthOfField=8; } // reset if ray is facing left or right it doesnt check anything
        // check if ray hits something  (depth of field thing to prevent endless loops)
        while(depthOfField<8) {
            mapX= (int)rayX>>6;
            mapY= (int)rayY>>6;
            mapTile = mapY*mapGridWidth + mapX;
            // breaks if ray hits wall
            if (mapTile > 0 && mapTile<mapGridWidth*mapGridHeight && wallMap[mapTile]>0) {
                horiX = rayX;
                horiY = rayY;
                horiDistance=sqrt((horiX-playerX)*(horiX-playerX)+(horiY-playerY)*(horiY-playerY));
                // subtract 1 bc computers count from zero and it skips the first texture if counted from 1
                mapTileValueH=wallMap[mapTile]-1;

                depthOfField = 8;
            } else { rayX += xOffset; rayY += yOffset; depthOfField += 1; } // keep checking
        }

        // ----------------------------- vertical ray check -------------------------------
        float nTan = -tan(rayAngle);
        depthOfField = 0;
        float vertDistance = 100000000000000000, vertX=playerX,vertY=playerY;
        // check if ray hits vertical line
        if (rayAngle > PI2 && rayAngle < PI3) {
            rayX = (((int)playerX>>6)<<6)-0.001;
            rayY = (playerX-rayX) * nTan+playerY;
            xOffset =-64;
            yOffset = -xOffset*nTan;
        }
        if (rayAngle < PI2 || rayAngle > PI3) {
            rayX = (((int)playerX>>6)<<6)+64;
            rayY = (playerX-rayX) * nTan+playerY;
            xOffset = 64;
            yOffset = -xOffset*nTan;
        }
        // reset if the ray is facing up or down
        if (rayAngle == 0 || rayAngle == PI) { rayX = playerX; rayY = playerY; depthOfField=8; }
        // check if ray hits something  (depth of field thing to prevent endless loops)
        while(depthOfField<8) {
            mapX= (int)rayX>>6;
            mapY= (int)rayY>>6;
            mapTile = mapY*mapGridWidth + mapX;
            // DO NOT REMOVE FOLLOWING LINE OF CODE, CAUSES MEMORY ISSUES
            if (mapTile < 0) {mapTile = 0;} // important line of code that makes sure that the map tile cant be below zero
            // breaks if ray hits wall
            if (mapTile<mapGridWidth*mapGridHeight && wallMap[mapTile]>0) {
                vertX = rayX;
                vertY = rayY;
                vertDistance=sqrt((vertX-playerX)*(vertX-playerX)+(vertY-playerY)*(vertY-playerY));
                // subtract 1 bc computers count from zero and it skips the first texture if counted from 1
                mapTileValueV = wallMap[mapTile]-1;

                depthOfField = 8;
            } else { rayX += xOffset; rayY += yOffset; depthOfField += 1; } // keep checking
        }

        // choose to show shortest line
        if ( horiDistance < vertDistance ) { // choose horizontal line
            rayX = horiX; rayY = horiY;
            distance = horiDistance;
            shade = 1.0f;
        }
        if ( horiDistance > vertDistance ) { // choose vertical ray
            rayX = vertX; rayY = vertY;
            distance = vertDistance;
            mapTileValueH = mapTileValueV;
            shade = 0.7f;
        }

        glColor3f(0.0f, 0.0f, 1.0f);
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2i( playerX, playerY );
        glVertex2i( rayX, rayY );
        glEnd();


        // -------------------------- actually draw the game -----------------------------
        // all might function to fix distortion
        float ca = playerAngle-rayAngle;
        distance = distance * cos(ca);

        // decide on the line height based off the distance to draw perspective
        float lineHeight = (mapGridSize*320)/distance;

        // decide offset for how much higher the next pixel in the line should be drawn
        float texYStep = textureResolution/lineHeight;
        float texYOff = 0;
        // clip line height to the screen
        if (lineHeight > 320) {
            texYOff = (lineHeight - 320)/2.0;
            lineHeight = 320;
        }
        // set high the lines should be drawn
        float lineOffset = 160-lineHeight/2;

        float textureY=texYOff*texYStep+mapTileValueH;

        // draw the textures X coordinate, if else statement to make sure it draws correctly on vertical & horizontal lines
        float textureX;
        if (shade == 1) {
            textureX=(int)(rayX/2.0)%textureResolution;
            // flip if textures are south
            if (rayAngle > PI) {
                textureX = 31 - textureX;
            }
        } else {
            textureX = (int)(rayY/2.0)%textureResolution;
            // flip if textures are west (left)
            if (rayAngle > PI2 && rayAngle < PI3) {
                textureX = 31 - textureX;
            }
        }
        int y;

        // draw vertical lines one pixel at a time
        for (y=0; y < lineHeight; y++) {
            // pick pixel from the array,
            //float c = All_Textures[static_cast<int>(textureY)*32 + static_cast<int>(textureX)]*shade;
            int pixel = (static_cast<int>(textureY)*textureResolution+static_cast<int>(textureX))*3+mapTileValueH*32*32*3;
            int red = allTextures[pixel + 0]*shade;
            int green = allTextures[pixel + 1]*shade;
            int blue = allTextures[pixel + 2]*shade;
            glColor3ub(red,green,blue);
            glLineWidth(8);
            glBegin(GL_POINTS);
            glVertex2i(r*8+530,y+lineOffset);
            glEnd();
            textureY += texYStep;
        }

        // -------------------- floor & ceiling rendering --------------------
        for (y=lineOffset + lineHeight; y < 320; y++) {
            // precalculate the texture coords using some formula i dont understand
            float dy = y-(320/2.0), raFix=cos(playerAngle-rayAngle);
            textureX = playerX / 2 + cos(rayAngle)*158*32 / dy / raFix;
            textureY = playerY / 2 + sin(rayAngle)*158*32 / dy / raFix;

            int mp = floorMap[static_cast<int>(textureY / 32.0)*mapGridWidth+static_cast<int>(textureX / 32.0)]*textureResolution*textureResolution;
            int pixel = (static_cast<int>(textureY)&31)*32 + (static_cast<int>(textureX)&31)*3;//+mp*3;
            int red = allTextures[pixel + 0];
            int green = allTextures[pixel + 1];
            int blue = allTextures[pixel + 2];
            // look up coords in the texture array
            float c = All_Textures[((int)(textureY)&31)*32 + ((int)(textureX)&31)+mp]*0.7;
            //glColor3f(c,c,c);
            glColor3ub(red,green,blue);
            glBegin(GL_POINTS);
            glVertex2i(r*8+530,y);
            glEnd();

            // ceiling (its the same as the floor except that the y on the glVertex is different
            mp = floorMap[static_cast<int>(textureY / 32.0)*mapGridWidth+static_cast<int>(textureX / 32.0)]*textureResolution*textureResolution;
            pixel = (static_cast<int>(textureY)&31)*textureResolution + (static_cast<int>(textureX)&31)*3+mp*3;
            red = allTextures[pixel + 0];
            green = allTextures[pixel + 1];
            blue = allTextures[pixel + 2];
            // look up coords in the texture array
            c = All_Textures[((int)(textureY)&31)*32 + ((int)(textureX)&31)+mp]*0.7;
            glColor3f(c,c,c);
            //glColor3ub(red,green,blue);
            glPointSize(8);
            glBegin(GL_POINTS);
            glVertex2i(r*8+530,320-y);
            glEnd();
        }
        float angleIncrease = std::atan(rayX - 160) / 100;
        rayAngle = rayAngle - rayResoultion * DR;
        if (rayAngle < 0)    { rayAngle += 2*PI; }
        if (rayAngle > 2*PI) { rayAngle -= 2*PI; }
    }
}

int main() {
    // following line is just in case i want some segmentation errors, to avoid having too much fun.
    // std::cout << floorMap[-100000000000000] << std::endl;
    // glfw boilerplate
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // create the window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "", nullptr, nullptr);
    // error handling for window
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // check if GLAD is installed
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    // makes it orthographic bc perspective doesnt work for not really 3d game
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0, SCR_WIDTH, SCR_HEIGHT, 0, -1, 1);

    playerX = playerY = 300;
    playerDirectionX = cos(playerAngle);
    playerDirectionY = sin(playerAngle);

    double lastTime = glfwGetTime();

    std::ostringstream oss;
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();

        // cap game at the frame limit (default 60fps)
        if (currentTime - lastTime >= fpsLimit) {
            processInput(window);
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            drawMap2D();
            drawPlayer2D();
            drawRays();
            glfwSwapBuffers(window);
            glfwPollEvents();
            lastTime = currentTime;
        }
    }
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    int xOffset = 0;
    if (playerDirectionX > 0) { xOffset = 20; } else { xOffset = -20; }
    int yOffset = 0;
    if (playerDirectionY > 0) { yOffset = 20; } else { yOffset = -20; }

    int pGridPosX = playerX/64; int pGridPosY = playerY/64;
    int pGridPosXOff = (playerX+xOffset)/64; int pGridPosYOff = (playerY+yOffset)/64;
    int pGridPosXOffSub = (playerX-xOffset)/64; int pGridPosYOffSub = (playerY-yOffset)/64;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (wallMap[pGridPosY*mapGridWidth+pGridPosXOff] == 0 ) {
            playerX += playerDirectionX*playerMoveSpeed;
        }
        if (wallMap[pGridPosYOff*mapGridWidth+pGridPosX] == 0 ) {
            playerY += playerDirectionY*playerMoveSpeed;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (wallMap[pGridPosX*mapGridWidth+pGridPosXOffSub] == 0 ) {
            playerX -= playerDirectionX*playerMoveSpeed;
        }
        if (wallMap[pGridPosYOffSub*mapGridWidth+pGridPosX] == 0 ) {
            playerY -= playerDirectionY*playerMoveSpeed;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (wallMap[pGridPosY*mapGridWidth+pGridPosXOff] == 2 ) {
            wallMap[pGridPosY*mapGridWidth+pGridPosXOff] = 0;
        }
        if (wallMap[pGridPosYOff*mapGridWidth+pGridPosX] == 2 ) {
            wallMap[pGridPosYOff*mapGridWidth+pGridPosX] = 0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerAngle += playerTurnSpeed;
        if (playerAngle < 0) playerAngle += 2*std::numbers::pi;
        playerDirectionX = cos(playerAngle);
        playerDirectionY = sin(playerAngle);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerAngle -= playerTurnSpeed;
        if (playerAngle > 2*std::numbers::pi) playerAngle -= 2*std::numbers::pi;
        playerDirectionX = cos(playerAngle);
        playerDirectionY = sin(playerAngle);
    }
}