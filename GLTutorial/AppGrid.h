#pragma once
#include "Application.h"

#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <aie\Gizmos.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>

#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat4;

class AppGrid : public Application
{
public:
	AppGrid();

	bool startup();
	void shutdown();

	bool update(float deltaTime);
	void draw();


	GLFWwindow *window;
	mat4 view;
	mat4 projection;
};