#pragma once

#include <iostream>
#include <string>
using namespace std;

#include "pgr.h"

class Texture
{
public:
	GLuint texture;

	Texture(string texPath);
	~Texture();
};

