#pragma once
#include "Camera.h"
#include "Controller.h"

class FlyCamera : public Camera
{
public:
	float speed, rotationX, rotationY, sensitivity = 0;
	vec3 up, worldPosition = vec3(0);
	Controller control;

	void update(float dt);
	void setSpeed(float speed, float sensitivity);
};