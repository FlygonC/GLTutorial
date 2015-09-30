#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "FBXLoader.h"
#include "ShaderLoader.h"
//#include "Sky.h"
#include "SkyBox.h"

#include "GLUtilities.h"

class AppPostProcess : public Application
{
public:
	AppPostProcess();

	SimpleFrameBuffer render;
	FullScreenQuad screen;

	FBXLoader fbxData;
	FlyCamera camera;
	//Sky sky;
	SkyBox box;

	unsigned int FBXShader;

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

	void FBXDraw();
};