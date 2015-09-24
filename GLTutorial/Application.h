#pragma once
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

class Application 
{
private:
	float currentTime = 0;
	float deltaTime = 0;
	float lastTime = 0;
	vec3 skyColor = vec3(0);

	bool startGL();

	

protected:
	GLFWwindow *window;
	mat4 view;
	mat4 projection;

	virtual int  oninit() { return -99; }
	virtual void onkill() {}
	virtual bool onstep(float deltaTime) { return false; }
	virtual void ondraw() {}

	void setSky(vec3 color);

public:
	
	int  init();
	void kill();
	bool step();
	void draw();

	float getDeltaTime();
	float getCurrentTime();
	
	vec3 getSky();
};