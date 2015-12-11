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
	object1.data->mesh = "SoulSpear";
	object1.data->material.diffuseTexture = "SoulSpearDiffuse";
	object1.data->material.normalTexture = "SoulSpearNormal";
	object1.data->material.specularTexture = "SoulSpearSpecular";
	object1.data->material.specularPower = 8;
	object1.data->material.glowTexture = "Black";
	object1.data->transform.scale = glm::vec3(1.f);
	object1.data->transform.position = glm::vec3(0.f, 0.f, 0.f);

	object2.data->mesh = "Cube";
	object2.data->material.diffuseTexture = "Test";
	object2.data->material.normalTexture = "Flat";
	object2.data->material.specularTexture = "Black";
	object2.data->material.glowTexture = "Black";
	object2.data->transform.scale = glm::vec3(10.f, 1.f, 10.f);
	object2.data->transform.position = glm::vec3(0.f, -2.f, 0.f);

	dLight1.data->color = glm::vec3(0.5f);

	pLight1.data->position = glm::vec3(0.f);
	pLight1.data->radius = 5.f;

	//emitter1.data->velocity = 0.3f;
	emitter1.data->size.start = 0.1f;
	emitter1.data->size.end = 0.f;
	emitter1.data->color.start = glm::vec3(0.6f, 0.6f, 1.f);
	emitter1.data->color.end = glm::vec3(0.f, 0.f, 1.f);
	emitter1.data->maxLife.start = 1.f;
	emitter1.data->maxLife.end = 5.f;
}

void TestApplication::onStep(float dTime)
{
	camera.update(dTime);
	RenderEngine::Renderer::instance().setCamera(camera);
	float time = Window::instance().getTime();

	object1.data->transform.rotation = glm::vec3(0.f, sin(time), 0.f);

	emitter1.data->transform.position = glm::vec3(sin(time) * 3.f, cos(time) * 3.f, -5.f);
}
