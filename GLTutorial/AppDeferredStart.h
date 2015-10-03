#pragma once

#include "Application.h"
#include "FlyCamera.h"
#include "FBXLoader.h"
#include "ShaderLoader.h"
#include "GLUtilities.h"


class AppDeferredStart : public Application
{
public:
	unsigned int GProgram, compProgram, lightProgram, pointLightProgram;
	GPassFrameBuffer gPass;
	LightFrameBuffer lightPass;
	FullScreenQuadS lightQuad;
	SimpleCube pointLightCube;
	FullScreenQuadS compositeQuad;


	PartScreenQuad testQuad;
	PartScreenQuad testQuad2;

	FBXLoader fbxData;
	FlyCamera camera;

	AppDeferredStart();

	int oninit();
	void onkill();

	bool onstep(float deltaTime);
	void ondraw();

	void drawDirectionalLight(const vec3 direction, const vec3 diffuse);
	void drawPointLight(glm::vec3 position, float radius, glm::vec3 color);
};