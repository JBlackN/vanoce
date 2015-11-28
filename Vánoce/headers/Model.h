#pragma once

#include "pgr.h"
#include "headers/shader.h"
#include "headers/texture.h"

class Model
{
public:
	Model(Shader *, Texture *, const int, const int, const int, const float *, const unsigned *);
	~Model();
private:
	Shader * shader;
	Texture * texture;
	GLuint vbo;
	GLuint ebo;
	GLuint vao;
	GLint drawCount;
public:
	void loadData(const int nAttrPerVert, const int nVert, const int nTri, const float * vertArray, const unsigned * triArray);
	Shader * getShader();
	Texture * getTexture();
	GLuint getVAO();
	GLint getDrawCount();
};

