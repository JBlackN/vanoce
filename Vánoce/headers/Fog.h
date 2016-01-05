//----------------------------------------------------------------------------------------
/**
* \file       Fog.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Fog class.
*
*  File contains Fog class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
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

