#pragma once

#include <iostream>
#include <string>
using namespace std;

#include "pgr.h"

class Shader
{
public:
	Shader(string, string);
	~Shader();
private:
	GLuint shaderProgram;
public:
	GLuint getProgram();
};

