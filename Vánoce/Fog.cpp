#include "headers\Fog.h"

Fog::Fog(float density, glm::vec4 color, bool enabled)
{
	this->density = density;
	this->color = color;
	this->enabled = enabled;
}

Fog::~Fog()
{
}
