#include "AppPlanets.h"

AppPlanets::AppPlanets() {}

bool AppPlanets::startup()
{
	if (!startGL())
	{
		return false;
	}

	//109 =			   695,500
	//mercury = 46,000,000,000

	//				  size			spin							distance		orbit
	planets[0] = Body(109.f,		1,				nullptr,		0,				0,					"Sun"); //planets[0].hasRings = false;
	planets[1] = Body(.38f,			24.f / 60.f,	&planets[0],	130.f,			365.f / 88.f,		"Mercury");
	planets[2] = Body(.95f,			24.f / 243.f,	&planets[0],	160.f,			365.f / 224.f,		"Venus");
	planets[3] = Body(1.f,			1.f,			&planets[0],	190.f,			1.f,				"Earth");
	planets[4] = Body(0.1f,			1.f,			&planets[3],	2.f,			365.f / 16.f,		"Moon");
	planets[5] = Body(.53f,			24.f / 24.3f,	&planets[0],	220.f,			365.f / 686.97f,	"Mars");
	planets[6] = Body(11.20f,		24.f / 9.8f,	&planets[0],	250.f,			365.f / 4331.572f,	"Jupiter");
	planets[7] = Body(9.45f,		24.f / 10.2f,	&planets[0],	270.f,			365.f / 10832.33f,	"Saturn"); planets[7].hasRings = true;
	planets[8] = Body(4.f,			24.f / 17.14f,	&planets[0],	300.f,			365.f / 30799.09f,	"Uranus");
	planets[9] = Body(3.88f,		24.f / 16.f,	&planets[0],	330.f,			365.f / 60190.f,	"Neptune");
	//planets.push_back(sun);

	return true;
}

void AppPlanets::shutdown()
{
	
	Gizmos::destroy();

	glfwDestroyWindow(window);
	glfwTerminate();
}

bool AppPlanets::update()
{
	if (!glfwWindowShouldClose(window))
	{



		return true;
	}
	else
	{
		return false;
	}
}

void AppPlanets::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	Gizmos::create();

	Gizmos::clear();
	/*Gizmos::addTransform(glm::mat4(1));
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}*/

	for (int i = 0; i < numberOfPlanets; i++)
	{
		planets[i].update(currentTime);
	}

	for (int i = 0; i < numberOfPlanets; i++)
	{
		Gizmos::addSphere(vec3(1), planets[i].radius, 20, 20, vec4(0.9f, 0.9f, 0.9f, 1), &planets[i].mfinal);
		if (planets[i].hasRings)
		{
			Gizmos::addRing((vec3)planets[i].mfinal[3] / 2, 13.f, 17.f, 20, vec4(1, 1, 1, 0), &planets[i].mfinal);
		}
	}

	Gizmos::draw(projection * view);


	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool AppPlanets::startGL()
{
	if (glfwInit() == false)
	{
		return false;
	}

	window = glfwCreateWindow(1600, 900, "Planets", NULL, NULL);

	if (window == NULL)
	{
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}
	const int major = ogl_GetMajorVersion();
	const int minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	//Gizmos::create();
	//mat4 view = glm::lookAt(vec3(10, 10, 10), vec3(0), vec3(0, 1, 0));
	//mat4 projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);

	Gizmos::create();
	view = glm::lookAt(vec3(600, 600, 600), vec3(0), vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 4000.f);

	return true;
}