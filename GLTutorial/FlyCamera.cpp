#include "FlyCamera.h"

void FlyCamera::update(float dt)
{
	control.update(dt);
	
	//keys moving
	if (control.keyDown(65))
	{
		worldPosition.x -= speed * dt;
	}
	if (control.keyDown(68))
	{
		worldPosition.x += speed * dt;
	}
	if (control.keyDown(87))
	{
		worldPosition.z -= speed * dt;
	}
	if (control.keyDown(83))
	{
		worldPosition.z += speed * dt;
	}
	if (control.keyDown(69))
	{
		worldPosition.y += speed * dt;
	}
	if (control.keyDown(81))
	{
		worldPosition.y -= speed * dt;
	}
	//mouse observing
	vec2 mouse;
	if (!control.keyDown(32))
	{
		mouse = control.trackMouse(true) * sensitivity;
	}

	rotationX += mouse.x;
	if (rotationX > 360)
	{
		rotationX -= 360;
	}
	if (rotationX < 0)
	{
		rotationX += 360;
	}

	rotationY += mouse.y;
	if (rotationY > 90)
	{
		rotationY = 90;
	}
	if (rotationY < -90)
	{
		rotationY = -90;
	}
	//application
	worldTransform = mat4(1);
	worldTransform *= glm::translate(worldPosition) * glm::rotate(glm::radians(rotationX), vec3(0, -1, 0)) * glm::rotate(glm::radians(rotationY), vec3(-1, 0, 0));
}
void FlyCamera::setSpeed(float pspeed, float psensitivity)
{
	speed = pspeed;
	sensitivity = psensitivity;
}