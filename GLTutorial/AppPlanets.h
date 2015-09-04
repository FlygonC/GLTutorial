#pragma once
#include "Application.h"
#include "FlyCamera.h"

class Body
{
public:
	Body() {}
	
	Body(float r, float s, Body *p, float d, float o, std::string name ="") 
	{
		radius = r;
		spin = s;
		parent = p;
		parentDistance = d;
		if (p)
		{
			position = p->position + vec3(p->parentDistance, 0, 0);
		}
		orbitSpeed = o;
	}

	void update(float dt)
	{	//Local Transforms
		if (spin > 0)
		{
			mlocal = glm::rotate(dt, vec3(0, spin, 0));
		}
		else
		{
			mlocal = mat4(1);
		}
		if (parent)
		{	//Parent transforms
			morbit = parent->morbit;
		}	//0 if no parent
		else morbit = glm::mat4(1);
		//realative transforms (to parent, parent * self)
		morbit *= glm::rotate(orbitSpeed * dt, vec3(0, 1, 0)) * glm::translate(vec3(parentDistance, 0, 0));
		//final transform
		mfinal = morbit * mlocal;
	}

	std::string name;
	float radius;
	glm::vec3 position;
	float spin;
	Body *parent;// = new Body();
	float parentDistance;
	float orbitSpeed;

	bool hasRings = false;

	glm::mat4 mlocal;
	glm::mat4 morbit;
	glm::mat4 mfinal;
};

class AppPlanets : public Application
{
public:
	AppPlanets();


	int numberOfPlanets = 10;
	Body planets[10];

	FlyCamera camera;
	int cameraFocus = 0;

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

};