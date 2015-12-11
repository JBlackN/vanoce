#include "headers\Light.h"

Light::Light(glm::vec3 position, glm::vec3 color)
{
	this->position = position;
	this->color = color;
}

Light::~Light()
{
}