#pragma once

#include "pgr.h"

class Fog
{
public:
	bool enabled;

	float density;
	glm::vec4 color;

	Fog(float density, glm::vec4 color, bool enabled = true);
	~Fog();
};

