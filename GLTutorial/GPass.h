#pragma once

#include "Render.h"

namespace RenderEngine
{
	class GPass : public AssetLibrary::RenderPass
	{
	public:
		GPass() {}
		void init();
		void prep();
		//void draw();
		void post();
	};
	class LightPass : public AssetLibrary::RenderPass
	{
	public:
		LightPass() {}
		void init();
		void prep();
		//void draw();
		void post();
	};
	class CompositePass : public AssetLibrary::RenderPass
	{
	public:
		CompositePass() {}
		void init();
		void prep();
		//void draw();
		void post();
	};
}