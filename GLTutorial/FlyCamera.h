#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
	float speed;
	vec3 up;
public:
	void update(float dt);
	void setSpeed(float s);
};