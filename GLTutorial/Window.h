#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

namespace AssetLibrary
{
	class Window
	{
		GLFWwindow *window;
		unsigned int width, height;

		Window() :window(nullptr), width(0), height(0) {}
	public:
		static Window &instance() { static Window a; return a; }

		bool build(unsigned int w, unsigned int h, const char* title);

		void step();

		void kill();

		float		getTime() const;
		bool		getKey(unsigned k) const;
		bool		getShouldClose() const;
		unsigned	getWidth() const;
		unsigned	getHeight() const;
		glm::mat4	getTexelAdjustmentMatrix() const;
	};
}