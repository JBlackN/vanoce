#pragma once

#include "pgr.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

class Model
{
public:
	Shader * shader;
	Material * material;
	Texture * texture;
	GLuint vbo;
	GLuint ebo;
	GLuint vao;
	int drawCount;

	Model(Shader * shader, Material * material, Texture * texture, const int drawCount);
	~Model();

	void loadData(const int nAttrPerVert, const int nVert, const int nTri, const float * vertices, const unsigned int * triangles);
};

