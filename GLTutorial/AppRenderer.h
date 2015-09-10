#pragma once
#include "Application.h"
#include "FlyCamera.h"

#include <tiny_obj_loader.h>
#include <vector>

struct Vertex
{
	vec4 position;
	vec4 color;
};

/*
typedef struct
{
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texcords;
	std::vector<unsigned int> index;
	std::vector<float> material_ids;
} mesh_t;

typedef struct
{
	std::string name;
	mesh_t mesh;
} shape_t;
*/

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
	
	std::string err;
	
	std::vector<OpenGLInfo> glInfo;

	unsigned int programID;

	FlyCamera camera;

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

	//void generateGrid(unsigned int prows, unsigned int columns);

	void createOpenGLBuffers(std::vector<tinyobj::shape_t>& shapes);
};