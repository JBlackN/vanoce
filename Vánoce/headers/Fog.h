#pragma once

#include "pgr.h"

/**
 * Class describes fog and its properties.
 */
class Fog
{
public:
	bool enabled;

	float density;
	glm::vec4 color;

	Fog(float density, glm::vec4 color, bool enabled = true);
	~Fog();
};

