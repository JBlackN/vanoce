#include "headers\Material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 emission)
{
	this->ambient = glm::vec4(ambient, 1);
	this->diffuse = glm::vec4(diffuse, 1);
	this->specular = glm::vec4(specular, 1);
	this->shininess = shininess;
	this->emission = glm::vec4(emission, 1);
}

Material::~Material()
{
}
