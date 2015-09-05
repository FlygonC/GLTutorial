#pragma once
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

using glm::vec2;

class Controller
{
	static bool keys[355];
	static bool last[355];
	//static bool repe[355];
	static vec2 mouseCurrent;
	static vec2 mouseDelta;

public:
	static void update(float dt);

	static bool keyPressed(int i);
	static bool keyDown(int i);
	//static bool keyReleased(int i);

	static vec2 trackMouse(bool lockMouse);
};