#pragma once

#include "AssetLibrary.h"
#include "FlyCamera.h"

using namespace RenderEngine;

class TestApplication : public AssetLibrary::Application
{
	void onInit();
	void onKill();
	void onPlay();
	void onStep(float dTime);

	FlyCamera camera;

	GeometryObject object1;
	GeometryObject object2;

	DirectionalLightObject dLight1;

	PointLightObject pLight1;

	ParticleEmitterObject emitter1;
};