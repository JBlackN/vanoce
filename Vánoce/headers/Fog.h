#pragma once

#include "pgr.h"

class Fog
{
public:
	float density;
	glm::vec4 color;
	bool enabled;

	Fog(float density, glm::vec4 color, bool enabled = true);
	~Fog();
};

