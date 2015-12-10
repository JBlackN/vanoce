#pragma once

#include <iostream>
#include <string>
using namespace std;

#include "pgr.h"

class Shader
{
public:
	GLuint shaderProgram;

	Shader(string vsPath, string fsPath);
	~Shader();
};

