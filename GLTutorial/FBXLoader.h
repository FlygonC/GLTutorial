#pragma once

#include <gl_core_4_4.h>

#include <FBXFile.h>
#include <stb_image.h>
#include <vector>

class FBXLoader
{
public:
	FBXFile file;

	struct OpenGLInfo
	{
		unsigned int VAO, VBO, IBO, indexCount;
		unsigned int diffuse, normal, specular;
	};
	std::vector<OpenGLInfo> glInfo;


	void loadFBX(const char* path);
	void freeFBX();
	int getMeshCount();
	//FBXMeshNode getMeshByIndex(int index);
};