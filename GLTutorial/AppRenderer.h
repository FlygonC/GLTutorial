#pragma once

#include "Application.h"
#include "FlyCamera.h"

#include <stb_image.h>
#include <tiny_obj_loader.h>
#include <vector>

struct Vertex
{
	vec4 position;
	vec4 color;
};


struct OpenGLInfo
{
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	unsigned int indexCount;
};


class AppRenderer : public Application
{
public:
	AppRenderer();

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<OpenGLInfo> glInfo;

	std::string err;

	int imageWidth, imageHeight, imageFormat = 0;
	unsigned char* data;
	unsigned int texture = 0;

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	//unsigned int indexCount;

	unsigned int programID;

	FlyCamera camera;

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

	//void generateGrid(unsigned int prows, unsigned int columns);

	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);

	void createTexturedSquare();
};