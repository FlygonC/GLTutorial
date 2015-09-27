#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "FBXLoader.h"
#include "ShaderLoader.h"


class AppFBX : public Application
{
public:
	AppFBX();

	FBXLoader fbxData;

	unsigned int programID = 0;

	FlyCamera camera;

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

};