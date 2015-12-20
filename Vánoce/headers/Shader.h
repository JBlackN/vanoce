#pragma once

#include <iostream>
#include <string>
using namespace std;

#include "pgr.h"

class Shader
{
public:
	GLuint shaderProgram;
public:
	Shader(string vsPath, string fsPath);
	~Shader();
};

