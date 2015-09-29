#pragma once

#include <gl_core_4_4.h>

#include <stb_image.h>

#include "ShaderLoader.h"
#include "Camera.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	unsigned int VAO, VBO, IBO, programID, cubeMap;

	void create();
	void loadCubeMap(const char* right, const char* left, const char* top, const char* bottom, const char* back, const char* front);
	void draw(Camera camera);
};