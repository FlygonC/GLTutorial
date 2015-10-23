#pragma once

#include "Render.h"

namespace RenderEngine
{
	class GPass : public AssetLibrary::RenderPass
	{
		void init();
		void prep();
		void post();
	};
	class LightPass : public AssetLibrary::RenderPass
	{
		void init();
		void prep();
		void post();
	};
	class CompositePass : public AssetLibrary::RenderPass
	{
		void init();
		void prep();
		void post();
	};
}