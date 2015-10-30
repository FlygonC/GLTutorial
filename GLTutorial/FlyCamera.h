#pragma once
#include "Camera.h"
#include "Controller.h"

class FlyCamera : public Camera
{
public:
	float speed, sensitivity = 0;
	float rotationX = -45;
	float rotationY = 45;
	vec3 up;
	vec3 worldPosition = vec3(3,4,3);
	Controller control;

	void update(float dt);
	void setSpeed(float speed, float sensitivity);
};