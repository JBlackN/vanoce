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

