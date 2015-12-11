#pragma once

#include "pgr.h"

class Material
{
public:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emission;
	float shininess;

	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 emission = glm::vec3(0));
	Material(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess, glm::vec4 emission = glm::vec4(0));
	~Material();
};

