#pragma once

#include <list>

#include "AssetLibrary.h"

namespace AssetLibrary
{
	namespace UNIFORM
	{
		enum TYPE { NONE = 0, FLO1, FLO2, FLO3, FLO4, MAT4, INT1, TEX2, eSIZE };
	}

	class RenderPass
	{
	protected:
		Asset<ASSET::FBO> fbo;
		Asset<ASSET::SHADER> shader;
	public:
		RenderPass(Asset<ASSET::SHADER> a_shader, Asset<ASSET::FBO> a_fbo) : shader(a_shader), fbo(a_fbo) {}

		bool setUniform(const char* name, UNIFORM::TYPE type, const void* value, unsigned count = 1, bool normalize = false);

		virtual void prep() = 0;
		virtual void post() = 0;
	};
}