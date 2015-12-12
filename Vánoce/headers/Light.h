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

	enum LightType { point = 0, spot = 1, directional = 2 } type;
	bool enabled;

	Light(LightType type, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		glm::vec3 spot_direction, float spot_exponent, float spot_cutoff,
		float constant_attenuation, float linear_attenuation, float quadratic_attenuation,
		bool enabled = true);
	~Light();
};