#include "Controller.h"

bool Controller::keys[355];
bool Controller::last[355];

vec2 Controller::mouseCurrent = vec2(800, 450);
vec2 Controller::mouseDelta;

void Controller::update(float dt)
{
	auto window = glfwGetCurrentContext();
	for (int i = 0; i < 355; i++)
	{
		last[i] = keys[i];
		keys[i] = glfwGetKey(glfwGetCurrentContext(), i);
	}
}

bool Controller::keyPressed(int i) { return last[i] && !keys[i]; }
bool Controller::keyDown(int i) {return keys[i]; }
//bool Controller::keyReleased(int i) { return !last[i] && !keys[i]; }

vec2 Controller::trackMouse(bool lockMouse)
{
	double mouseTempX;
	double mouseTempY;
	vec2 result;
	glfwGetCursorPos(glfwGetCurrentContext(), &mouseTempX, &mouseTempY);
	mouseDelta = vec2((float)mouseTempX, (float)mouseTempY) - mouseCurrent;
	result = mouseDelta;
	
	if (lockMouse)
	{
		glfwSetCursorPos(glfwGetCurrentContext(), 800, 450);
		mouseCurrent = vec2(800, 450);
		//mouseDelta = mouseCurrent;
	}
	else
	{
		mouseCurrent = vec2((float)mouseTempX, (float)mouseTempY);
	}
	
	return result;
}