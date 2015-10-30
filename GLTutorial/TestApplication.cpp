#include "TestApplication.h"

using namespace AssetLibrary;

void TestApplication::onInit()
{
	camera.setPerspective(glm::pi<float>() * 0.5f, 9.f / 16.f, .5f, 200);
	camera.setSpeed(5, 0.1f);
	camera.setPosition(glm::vec3(0, 0, -4));

	cube.mesh = "Cube";
	cube.material.diffuseTexture = "Test";
	cube.material.normalTexture = "Flat";
	cube.material.specularTexture = "White";
	cube.material.specularPower = 32;

	dLight.color = glm::vec3(1, 1, 1);
	dLight.direction = glm::vec3(0.9f, 1, 0.8f);

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
	pLight4.position = glm::vec3(-2, -2, -2);
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
	//dLight.instantiate();
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
	cube.transform.rotation = glm::vec3(time, time, time);
	cube.transform.scale = glm::vec3(sin(time));
	//cube.transform.position.z = 0;
	cube.update();
	//dLight.update();
	pLight.update();
	pLight2.update();
	pLight3.update();
	pLight4.update();
}
