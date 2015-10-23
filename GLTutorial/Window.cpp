#include <gl_core_4_4.h>
#include "Window.h"

bool AssetLibrary::Window::build(unsigned int w, unsigned int h, const char* title)
{
	if (glfwInit() == false)
	{
		return false;
	}

	width = w;
	height = h;
	window = glfwCreateWindow(w, h, title, NULL, NULL);

	if (window == NULL)
	{
		return false;
	}

	glfwMakeContextCurrent(window);

	ogl_LoadFunctions();

	return true;
}

void AssetLibrary::Window::step()
{
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void AssetLibrary::Window::kill()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

float AssetLibrary::Window::getTime() const
{
	return glfwGetTime();
}

bool AssetLibrary::Window::getKey(unsigned k) const
{
	return glfwGetKey(window, k);
}

bool AssetLibrary::Window::getShouldClose() const
{
	return glfwWindowShouldClose(window);
}

unsigned AssetLibrary::Window::getWidth() const
{
	return width;
}

unsigned AssetLibrary::Window::getHeight() const
{
	return height;
}

glm::mat4 AssetLibrary::Window::getTexelAdjustmentMatrix() const
{
	return glm::mat4();
}
