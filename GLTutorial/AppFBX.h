#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "FBXLoader.h"
#include "ShaderLoader.h"

//#include <aie\Gizmos.h>

#include <stb_image.h>
#include <FBXFile.h>
//#include <tiny_obj_loader.h>
#include <vector>
#include <fstream>

class AppFBX : public Application
{
public:
	AppFBX();

/*	struct OpenGLInfo
	{
		unsigned int VAO;
		unsigned int VBO;
		unsigned int IBO;
		unsigned int indexCount;
	};
	

	FBXFile file;

	std::vector<OpenGLInfo> glInfo;
*/
	FBXLoader fbxData;

	//int imageWidth, imageHeight, imageFormat = 0;
	//unsigned char* data;
	unsigned int programID = 0;

	FlyCamera camera;

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

	//void loadFBX(const char* path);
	//void freeFBX();
	//int getMeshCount();
	//FBXMeshNode getMeshByIndex(int index);

	//unsigned int loadTexture(const std::string path);
};