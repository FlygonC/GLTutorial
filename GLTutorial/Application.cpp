#include "Application.h"

bool Application::startGL()
{
	if (glfwInit() == false)
	{
		return false;
	}

	window = glfwCreateWindow(1600, 900, "Planets", NULL, NULL);

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

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);

	view = glm::lookAt(vec3(109 * 6, 109 * 6, 0), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 4000.f);

	return true;
}

int Application::init()
{
	if (!startGL())
	{
		return -1;
	}
	if (!oninit())
	{
		return -2;
	}
	return true;
}

void Application::kill()
{
	onkill();

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Application::step()
{
	
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	if (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		return onstep(deltaTime);
	}
	else
	{
		return false;
	}
}

void Application::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ondraw();
	glfwSwapBuffers(window);
}

float Application::getDeltaTime()
{
	return deltaTime;
}
float Application::getCurrentTime()
{
	return currentTime;
}