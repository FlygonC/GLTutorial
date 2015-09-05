#pragma once
#include "Camera.h"
#include "Controller.h"

class FlyCamera : public Camera
{
	float speed, rotationX, rotationY, sensitivity = 0;
	vec3 up, worldPosition = vec3(0);
	Controller control;

public:
	void update(float dt);
	void setSpeed(float speed, float sensitivity);
};