#pragma once

#include "AssetManager.h"
#include "Window.h"
#include "RenderEngine.h"

namespace AssetLibrary
{
	class Application
	{
	protected:
		virtual void onInit() {};
		virtual void onKill() {};
		virtual void onPlay() {};
		virtual void onStep(float dTime) {};
	public:
		float currentTime, deltaTime, lastTime;
		void init(int width = 1280, int height = 720, const char* name = "Application")
		{
			Window::instance().build(width, height, name);
			AssetManager::instance().init();
			RenderEngine::Renderer::instance().init();
			onInit();
		}

		void kill()
		{
			onKill();
			AssetManager::instance().kill();
			Window::instance().kill();
			RenderEngine::Renderer::instance().kill();
		}

		void play()
		{
			onPlay();
			do { 
				step();
				RenderEngine::Renderer::instance().render(deltaTime);
			} 
			while (Window::instance().getShouldClose() == false);
		}

		void step()
		{
			Window::instance().step();
			currentTime = glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;
			onStep(deltaTime);
		}
	};
}