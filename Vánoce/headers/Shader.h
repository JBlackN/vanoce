//----------------------------------------------------------------------------------------
/**
* \file       Shader.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Shader class.
*
*  File contains Shader class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <string>
using namespace std;

#include "pgr.h"

/**
 * Class holds shader program (consisting of vertex shader and fragment shader).
 */
class Shader
{
public:
	GLuint shaderProgram;
public:
	Shader(string vsPath, string fsPath);
	~Shader();
};

