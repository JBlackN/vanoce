#pragma once

#include "pgr.h"
#include "Shader.h"
#include "Texture.h"

class Model
{
public:
	Shader * shader;
	Texture * texture;
	GLuint vbo;
	GLuint ebo;
	GLuint vao;
	int nVertices;

	Model(Shader * shader, Texture * texture, const int nVertices);
	~Model();

	void loadData(const int nAttrPerVert, const int nVert, const int nTri, const float * vertices, const unsigned int * triangles);
};

