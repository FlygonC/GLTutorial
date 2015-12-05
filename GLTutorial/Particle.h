#pragma once

#include <glm\glm.hpp>

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	float size;
	float lifeTime = 1;
	float maxLife = 1;

};