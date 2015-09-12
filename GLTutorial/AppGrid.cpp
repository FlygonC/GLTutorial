#include "AppGrid.h"

AppGrid::AppGrid() {}

bool AppGrid::startup()
{
	if (glfwInit() == false)
	{
		return false;
	}

	window = glfwCreateWindow(800, 600, "Window!!!", NULL, NULL);

	if (window == NULL)
	{
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}
	const int major = ogl_GetMajorVersion();
	const int minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	//Gizmos::create();
	//mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	//mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	glClearColor(0.25f, 0.25f, 0.25f, 1);

	Gizmos::create();
	view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	return true;
}

void AppGrid::shutdown()
{
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool AppGrid::update()
{
	if (!glfwWindowShouldClose(window))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AppGrid::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	Gizmos::create();

	Gizmos::clear();
	Gizmos::addTransform(glm::mat4(1));
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}
	Gizmos::draw(projection * view);

	  
	glfwSwapBuffers(window);
	glfwPollEvents();
}