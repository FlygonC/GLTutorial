#include "AppPlanets.h"

AppPlanets::AppPlanets() {}

int AppPlanets::oninit()
{
	camera.setPerspective(glm::pi<float>() * 0.25f, 9.f/16.f, .5f, 4000);
	//camera.setLookAt(vec3(200, 200, 0), vec3(0, 0, -1), vec3(0, 1, 0));
	camera.setPosition(vec3(0, 200, 450));
	camera.setSpeed(100, 0.5);

	Gizmos::create();

	//				  size			spin							distance		orbit
	planets[0] = Body(109.f,		0,				nullptr,		0,				0,					"Sun"); //planets[0].hasRings = false;
	planets[1] = Body(.38f,			24.f / 60.f,	&planets[0],	130.f,			365.f / 88.f,		"Mercury");
	planets[2] = Body(.95f,			24.f / 243.f,	&planets[0],	160.f,			365.f / 224.f,		"Venus");
	planets[3] = Body(1.f,			1.f,			&planets[0],	190.f,			1.f,				"Earth");
	planets[4] = Body(0.33f,		1.f,			&planets[3],	3.f,			365.f / 16.f,		"Moon");
	planets[5] = Body(.53f,			24.f / 24.3f,	&planets[0],	220.f,			365.f / 686.97f,	"Mars");
	planets[6] = Body(11.20f,		24.f / 9.8f,	&planets[0],	250.f,			365.f / 4331.572f,	"Jupiter");
	planets[7] = Body(9.45f,		24.f / 10.2f,	&planets[0],	270.f,			365.f / 10832.33f,	"Saturn"); planets[7].hasRings = true;
	planets[8] = Body(4.f,			24.f / 17.14f,	&planets[0],	300.f,			365.f / 30799.09f,	"Uranus");
	planets[9] = Body(3.88f,		24.f / 16.f,	&planets[0],	330.f,			365.f / 60190.f,	"Neptune");
	//planets.push_back(sun);

	return true;
}

void AppPlanets::onkill()
{
	Gizmos::destroy();
}

bool AppPlanets::onstep(float deltaTime)
{
	for (int i = 0; i < numberOfPlanets; i++)
	{
		planets[i].update(getCurrentTime());
	}

	camera.update(deltaTime);

	return true;
}

void AppPlanets::ondraw()
{

	Gizmos::clear();
	
	for (int i = 0; i < numberOfPlanets; i++)
	{
		Gizmos::addSphere(vec3(1), planets[i].radius, 20, 20, vec4(0.8f, 0.8f, 0.8f, 1), &planets[i].mfinal);
		if (planets[i].hasRings)
		{
			Gizmos::addRing((vec3)planets[i].mfinal[3] / 2, 13.f, 17.f, 20, vec4(1, 1, 1, 0), &planets[i].mfinal);
		}
	}

	Gizmos::addRing(vec3(0,  10, 0), 500.f, 505.f, 40, vec4(1, 1, 1, 0));
	Gizmos::addRing(vec3(0,   0, 0), 500.f, 505.f, 40, vec4(1, 1, 1, 0));
	Gizmos::addRing(vec3(0, -10, 0), 500.f, 505.f, 40, vec4(1, 1, 1, 0));

	/*view = glm::lookAt(vec3(planets[cameraFocus].mfinal[3][0] + (planets[cameraFocus].radius * 5), 
		planets[cameraFocus].mfinal[3][1] + (planets[cameraFocus].radius * 5), planets[cameraFocus].mfinal[3][2]), 
		(vec3)planets[cameraFocus].mfinal[3], vec3(0, 1, 0));*/

	Gizmos::draw(camera.getProjectionView());

}

