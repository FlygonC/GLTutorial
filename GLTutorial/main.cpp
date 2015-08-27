#include "gl_core_4_4.h"
#include "GLFW\glfw3.h"

void main()
{
	glfwInit();

	auto window = glfwCreateWindow(800, 600, "Window!!!", NULL, NULL);

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) 
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}