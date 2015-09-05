#pragma once
#include "Application.h"
#include "FlyCamera.h"

struct Vertex
{
	vec4 position;
	vec4 color;
};

class AppRenderer : public Application
{
public:
	AppRenderer();
	
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	unsigned int rows;
	unsigned int cols;

	unsigned int programID;

	FlyCamera camera;

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

	void generateGrid(unsigned int prows, unsigned int columns);
};