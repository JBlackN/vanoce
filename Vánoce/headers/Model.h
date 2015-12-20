#pragma once

#include <vector>
using namespace std;

#include "pgr.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

class Model
{
public:
	Shader * shader;
	Material * material;
	vector<Texture *> textures;
	GLuint vbo;
	GLuint ebo;
	GLuint vao;

	int nAttrPerVert;
	int nVert;
	int nTri;
	const float * vertices;
	const unsigned int * triangles;

	int drawCount;
public:
	Model(Shader * shader, Material * material, Texture * texture,
		const int nAttrPerVert, const int nVert, const int nTri, const float * vertices, const unsigned int * triangles);
	Model(Shader * shader, Material * material, vector<Texture *> textures,
		const int nAttrPerVert, const int nVert, const int nTri, const float * vertices, const unsigned int * triangles);
	~Model();
private:
	void loadData();
};

