#include "TestApplication.h"

using namespace AssetLibrary;

void TestApplication::onInit()
{
	AssetManager::instance().loadTexture("BlockNorm", "../resources/blocknormal.png");
	AssetManager::instance().loadFBX("SoulSpear", "../fbx_models/soulspear/soulspear.fbx");

	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 200);
	camera.setSpeed(5, 0.1f);
	camera.setPosition(glm::vec3(0, 0, -4));

	cube.mesh = "SoulSpear";
	cube.material.diffuseTexture = "SoulSpearDiffuse";
	cube.material.normalTexture = "SoulSpearNormal";
	cube.material.specularTexture = "SoulSpearSpecular";
	cube.material.specularPower = 8;

	ground.mesh = "Cube";
	ground.material.diffuseTexture = "Test";
	ground.material.normalTexture = "Flat";
	ground.material.specularTexture = "Black";
	ground.material.specularPower = 0;
	ground.transform.rotation.x = glm::radians(10.f);
	ground.transform.scale = glm::vec3(10, 0.1f, 10);
	ground.transform.position.y = -2;

	dLight.color = glm::vec3(0.1f, 0.1f, 0.1f);
	dLight.direction = glm::vec3(1.f, 1.f, 1.f);

	pLight.color = glm::vec3(1, 0.3f, 0.3f);
	pLight.position = glm::vec3(-2, 2, -2);
	pLight.radius = 5;

	pLight2.color = glm::vec3(0.3f, 1, 0.3f);
	pLight2.position = glm::vec3(2, 2, 2);
	pLight2.radius = 5;

	pLight3.color = glm::vec3(0.3f, 0.3f, 1);
	pLight3.position = glm::vec3(2, -2, 2);
	pLight3.radius = 5;

	pLight4.color = glm::vec3(1, 1, 1);
	pLight4.position = glm::vec3(0, -2.5f, 0);
	pLight4.radius = 5;
}

void TestApplication::onKill()
{
	cube.destroy();
	dLight.destroy();
	pLight.destroy();
}

void TestApplication::onPlay()
{
	cube.instantiate();
	ground.instantiate();

	dLight.instantiate();

	pLight.instantiate();
	pLight2.instantiate();
	pLight3.instantiate();
	pLight4.instantiate();
}

void TestApplication::onStep(float dTime)
{
	camera.update(dTime);
	//camera.setPosition(glm::vec3(0, 0, -4));
	RenderEngine::Renderer::instance().setCamera(camera);

	float time = Window::instance().getTime();
	//cube.transform.rotation = glm::vec3(time, time, time);
	//cube.transform.scale = glm::vec3(sin(time));
	//cube.transform.position.z = 0;
	cube.update();

	ground.update();

	dLight.update();

	pLight.update();
	pLight2.update();
	pLight3.update();
	pLight4.update();
}
