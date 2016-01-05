//----------------------------------------------------------------------------------------
/**
* \file       Material.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Material class.
*
*  File contains Material class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include "pgr.h"

/**
 * Class describes material and its properties.
 */
class Material
{
public:
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emission;
	float shininess;
public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, glm::vec3 emission = glm::vec3(0));
	~Material();
};

