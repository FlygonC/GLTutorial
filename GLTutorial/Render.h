#pragma once

#include <list>

#include "AssetManager.h"
#include "Window.h"

using namespace AssetLibrary;

namespace RenderEngine
{
	namespace UNIFORM
	{
		enum TYPE { NONE = 0, FLO1, FLO2, FLO3, FLO4, MAT4, INT1, TEX2, eSIZE };
	}

	class RenderPass
	{
	public:
		Asset<ASSET::FBO> fbo;
		Asset<ASSET::SHADER> shader;
	
		RenderPass() {}

		bool setUniform(const char* name, UNIFORM::TYPE type, const void* value, unsigned count = 1, bool normalize = false);

		virtual void prep() {};
		virtual void post() {};
	};
}