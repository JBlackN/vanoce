#pragma once

#include <string>
using namespace std;

#include "pgr.h"

class Texture
{
public:
	Texture(Shader *, string);
	~Texture();
private:
	GLuint texture;
public:
	GLuint getTexture();
};

