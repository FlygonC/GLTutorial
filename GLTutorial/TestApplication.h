#pragma once

#include "AssetLibrary.h"
#include "FlyCamera.h"

class TestApplication : public AssetLibrary::Application
{
	void onInit();
	void onKill();
	void onPlay();
	void onStep(float dTime);

	FlyCamera camera;

	RenderEngine::RenderObjectEx cube;
	RenderEngine::RenderObjectEx cube2;
	RenderEngine::RenderObjectEx ground;

	RenderEngine::DirectionalLightEx dLight;
	RenderEngine::DirectionalLightEx dLight2;

	RenderEngine::PointLightEx pLight;
	RenderEngine::PointLightEx pLight2;
	RenderEngine::PointLightEx pLight3;
	RenderEngine::PointLightEx pLight4;

	RenderEngine::ParticleEmitterEx emitter1;
};