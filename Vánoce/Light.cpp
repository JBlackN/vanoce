#include "headers\Light.h"

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	float constant_attenuation, float linear_attenuation, float quadratic_attenuation, bool directional)
{
	this->position = glm::vec4(position, directional ? 0 : 1);
	this->ambient = glm::vec4(ambient, 1);
	this->diffuse = glm::vec4(diffuse, 1);
	this->specular = glm::vec4(specular, 1);

	this->constant_attenuation = constant_attenuation;
	this->linear_attenuation = linear_attenuation;
	this->quadratic_attenuation = quadratic_attenuation;
}

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 spot_direction, float spot_exponent, float spot_cutoff,
	float constant_attenuation, float linear_attenuation, float quadratic_attenuation, bool directional)
{
	this->position = glm::vec4(position, directional ? 0 : 1);
	this->ambient = glm::vec4(ambient, 1);
	this->diffuse = glm::vec4(diffuse, 1);
	this->specular = glm::vec4(specular, 1);

	this->spot_direction = spot_direction;
	this->spot_exponent = spot_exponent;
	this->spot_cutoff = spot_cutoff;

	this->constant_attenuation = constant_attenuation;
	this->linear_attenuation = linear_attenuation;
	this->quadratic_attenuation = quadratic_attenuation;
}

Light::~Light()
{
}