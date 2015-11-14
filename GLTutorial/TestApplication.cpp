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

	cube.mesh = "SoulSpear";
	cube.material.diffuseTexture = "SoulSpearDiffuse";
	cube.material.glowTexture = "Black";
	cube.material.glowTint = glm::vec3(0.1f, 0.5f, 0.5f);
	cube.material.normalTexture = "SoulSpearNormal";
	cube.material.specularTexture = "SoulSpearSpecular";
	cube.material.specularPower = 4;
	cube.transform.scale = glm::vec3(1.f);

	cube2.mesh = "Cube";
	cube2.material.diffuseTexture = "Black";
	cube2.material.glowTexture = "Black";
	cube2.material.normalTexture = "OrbNorm";
	cube2.material.specularTexture = "White";
	cube2.material.specularPower = 8;
	cube2.material.specularTint = glm::vec3(0.5f, 0.1, 1.f);
	cube2.transform.position = glm::vec3(3, 0.5f, 3);
	cube2.transform.scale = glm::vec3(0.5f);

	ground.mesh = "Cube";
	ground.material.diffuseTexture = "Grid";
	ground.material.diffuseTint = glm::vec3(1.f, 0.f, 0.f);
	ground.material.glowTexture = "Black";
	ground.material.normalTexture = "Flat";
	ground.material.specularTexture = "White";
	ground.material.specularPower = 0;
	ground.transform.rotation.x = glm::radians(0.f);
	ground.transform.scale = glm::vec3(30, 1.0f, 30);
	ground.transform.position.y = -1;

	//dLight.color = glm::vec3(0.9f);
	dLight.direction = glm::vec3(0.f, 1.f, -1.0f);

	dLight2.color = glm::vec3(0.5f, 0.5f, 0.5f);
	dLight2.direction = glm::vec3(1.0f, 1.0f, 0.5f);

	//pLight.color = glm::vec3(1.f, 0.3f, 0.3f);
	pLight.position = glm::vec3(-2.f, 2.f, -2.f);
	pLight.radius = 10;

	//pLight2.color = glm::vec3(1.f, 1.f, 1.f);
	pLight2.position = glm::vec3(2.f, 2.f, 2.f);
	pLight2.radius = 20;

	//pLight3.color = glm::vec3(0.3f, 0.3f, 1.f);
	pLight3.position = glm::vec3(2.f, -2.f, 2.f);
	pLight3.radius = 5;

	//pLight4.color = glm::vec3(1.f, 1.f, 1.f);
	pLight4.position = glm::vec3(0.f, -2.5f, 0.f);
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
	cube2.instantiate();
	ground.instantiate();

	//dLight.instantiate();
	dLight2.instantiate();

	//pLight.instantiate();
	//pLight2.instantiate();
	//pLight3.instantiate();
	//pLight4.instantiate();
}

void TestApplication::onStep(float dTime)
{
	camera.update(dTime);
	//camera.setPosition(glm::vec3(0, 0, -4));
	RenderEngine::Renderer::instance().setCamera(camera);

	float time = Window::instance().getTime();
	cube.transform.rotation = glm::vec3(0, -time, 0);
	//cube.transform.scale = glm::vec3(sin(time));
	//cube.transform.position.z = 0;
	cube.update();

	cube2.update();

	ground.update();

	dLight.direction = glm::vec3(sin(time), 1.0f, cos(time));
	//dLight.direction = glm::vec3(0.0f, 1.0f, 0.5f);
	//dLight.update();

	dLight2.update();

	//pLight.update();
	//pLight2.update();
	//pLight3.update();
	//pLight4.update();
}
