#pragma once

#include <glm\glm.hpp>

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	//float size;
	float lifeTime	= 1;
	float maxLife	= 1;

	Particle() : position(0), velocity(0), lifeTime(0), maxLife(1)
	{

	}

};