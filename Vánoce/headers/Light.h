#pragma once

#include "pgr.h"

class Light
{
public:
	glm::vec3 position;
	glm::vec3 color;

	Light(glm::vec3 position, glm::vec3 color);
	~Light();
};