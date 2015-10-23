#pragma once

#include "AssetLibrary.h"

namespace AssetLibrary
{
	class Application
	{
	protected:
		virtual void onInit() {};
		virtual void onKill() {};
		virtual void onPlay() {};
		virtual void onStep() {};
	public:
		void init(int width = 1280, int height = 720, const char* name = "Application")
		{
			Window::instance().build(width, height, name);
			AssetManager::instance().init();
			onInit();
		}

		void kill()
		{
			onKill();
			AssetManager::instance().kill();
			Window::instance().kill();
		}

		void play()
		{
			onPlay();
			do { step(); } while (Window::instance().getShouldClose() == false);
		}

		void step()
		{
			Window::instance().step();
			onStep();
		}
	};
}