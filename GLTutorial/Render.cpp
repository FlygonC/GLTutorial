#include "Render.h"

bool AssetLibrary::RenderPass::setUniform(const char * name, UNIFORM::TYPE type, const void * value, unsigned count, bool normalize)
{
	int loc = glGetUniformLocation(AssetManager::instance().get<ASSET::SHADER>(shader.name.c_str()), name);
	switch (type)
	{
	case UNIFORM::FLO1: glUniform1fv(loc, 1, (GLfloat*)value); break;
	case UNIFORM::FLO2: glUniform2fv(loc, 1, (GLfloat*)value); break;
	case UNIFORM::FLO3: glUniform3fv(loc, 1, (GLfloat*)value); break;
	case UNIFORM::FLO4: glUniform4fv(loc, 1, (GLfloat*)value); break;
	case UNIFORM::MAT4: glUniformMatrix4fv(loc, 1, normalize, (GLfloat*)value); break;
	case UNIFORM::INT1: glUniform1i(loc, (GLint)value); break;
	case UNIFORM::TEX2: glUniform1i(loc, (GLint)value); break;
	}

	return true;
}
