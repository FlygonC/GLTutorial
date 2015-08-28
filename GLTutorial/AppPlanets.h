#pragma once
#include "Application.h"

#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <aie\Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <iostream>
#include <list>

using glm::vec3;
using glm::vec4;
using glm::mat4;

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
		mlocal = glm::rotate(dt, vec3(0, spin, 0));
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

	bool startup();
	void shutdown();

	bool update();
	void draw();

	bool startGL();
	GLFWwindow *window;
	mat4 view;
	mat4 projection;

	float currentTime;
	float deltaTime;
	float lastTime;

	//std::list<Body> planets;
	int numberOfPlanets = 10;
	Body planets[10];
	Body sun, mars;
};