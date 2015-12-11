#pragma once

#include "pgr.h"

class Light
{
public:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	glm::vec4 position;

	glm::vec3 spot_direction;
	float spot_exponent;
	float spot_cutoff;

	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;

	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant_attenuation = 0, float linear_attenuation = 0, float quadratic_attenuation = 1.5f, bool directional = false);
	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		glm::vec3 spot_direction, float spot_exponent, float spot_cutoff,
		float constant_attenuation = 0, float linear_attenuation = 0, float quadratic_attenuation = 1.5f, bool directional = false);
	~Light();
};