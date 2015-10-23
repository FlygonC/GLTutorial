#include "TestApplication.h"

using namespace AssetLibrary;

void TestApplication::onInit()
{
	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 200);
	camera.setSpeed(5, 0.1f);
}

void TestApplication::onKill()
{

}

void TestApplication::onPlay()
{

}

void TestApplication::onStep()
{

}
