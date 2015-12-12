#include "headers\Light.h"

Light::Light(LightType type, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 spot_direction, float spot_exponent, float spot_cutoff,
	float constant_attenuation, float linear_attenuation, float quadratic_attenuation,
	bool enabled)
{
	this->type = type;
	this->enabled = enabled;

	this->position = glm::vec4(position, type == directional ? 0 : 1);
	this->ambient = glm::vec4(ambient, 1);
	this->diffuse = glm::vec4(diffuse, 1);
	this->specular = glm::vec4(specular, 1);

	this->spot_direction = spot_direction;
	this->spot_exponent = type != spot ? 1 : spot_exponent;
	this->spot_cutoff = type == point ? 180 : spot_cutoff;

	this->constant_attenuation = constant_attenuation;
	this->linear_attenuation = linear_attenuation;
	this->quadratic_attenuation = quadratic_attenuation;
}

Light::~Light()
{
}