#pragma once

#include <gl_core_4_4.h>

#include <stb_image.h>

#include "ShaderLoader.h"

class Sky
{
public:
	Sky();
	~Sky();

	unsigned int VAO, VBO, IBO, programID, texture;
	float aspectX, aspectY, viewX, viewY;

	void create(float aspect_x, float aspect_y);
	void loadTexture(const char* path);
	void draw(float veiw_x, float view_y);
};