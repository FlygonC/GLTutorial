#include "TestApplication.h"

using namespace AssetLibrary;

void TestApplication::onInit()
{
	AssetManager::instance().loadTexture("BlockNorm", "../resources/blocknormal.png");
	AssetManager::instance().loadTexture("OrbNorm", "../resources/1426-normal.jpg");
	AssetManager::instance().loadTexture("Grid", "../resources/10grid.png");
	AssetManager::instance().loadTexture("TestGlow", "../resources/testglow.png");
	AssetManager::instance().loadFBX("SoulSpear", "../fbx_models/soulspear/soulspear.fbx");

	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 100);
	camera.setSpeed(5, 0.1f);
	//camera.setPosition(glm::vec3(0, 0, -4));


}

void TestApplication::onKill()
{
	
}

void TestApplication::onPlay()
{
	object1.data->mesh = "Cube";
	object1.data->material.diffuseTexture = "Test";
	object1.data->material.normalTexture = "Flat";
	object1.data->material.specularTexture = "Black";
	object1.data->material.glowTexture = "Black";
	object1.data->transform.scale = glm::vec3(1.f);

	//dLight1.data->

	pLight1.data->position = glm::vec3(-2, -2, -2);
	pLight1.data->radius = 3.f;

	//emitter1.data->velocity = 0.3f;
	emitter1.data->size = 1;
	emitter1.data->color = glm::vec3(1);
	emitter1.data->maxLife = 5.f;
}

void TestApplication::onStep(float dTime)
{
	camera.update(dTime);
	RenderEngine::Renderer::instance().setCamera(camera);
}
