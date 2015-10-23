#pragma once

#include "AssetLibrary.h"
#include "FlyCamera.h"

class TestApplication : public AssetLibrary::Application
{
	void onInit();
	void onKill();
	void onPlay();
	void onStep();

	FlyCamera camera;
};