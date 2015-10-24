#include "Render.h"

bool AssetLibrary::RenderPass::setUniform(const char * name, UNIFORM::TYPE type, const void * value, unsigned count, bool normalize)
{
	int loc = glGetUniformLocation(AssetManager::instance().get<ASSET::SHADER>(shader.name.c_str()), name);
	switch (type)
	{
	case UNIFORM::FLO1: glUniform1fv(loc, count, (GLfloat*)value); break;
	case UNIFORM::FLO2: glUniform2fv(loc, count, (GLfloat*)value); break;
	case UNIFORM::FLO3: glUniform3fv(loc, count, (GLfloat*)value); break;
	case UNIFORM::FLO4: glUniform4fv(loc, count, (GLfloat*)value); break;
	case UNIFORM::MAT4: glUniformMatrix4fv(loc, count, normalize, (GLfloat*)value); break;
	case UNIFORM::INT1: glUniform1i(loc, (GLint)value); break;
	case UNIFORM::TEX2: glActiveTexture(GL_TEXTURE0 + count);
						glBindTexture(GL_TEXTURE_2D, *(const GLuint*)value);
						glUniform1i(loc, count);
						break;
	default: return false;
	}

	return true;
}
