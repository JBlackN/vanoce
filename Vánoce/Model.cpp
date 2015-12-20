#include "headers\Model.h"

Model::Model(Shader * shader, Material * material, Texture * texture,
	const int nAttrPerVert, const int nVert, const int nTri, const float * vertices, const unsigned int * triangles)
{
	this->shader = shader;
	this->material = material;
	this->textures.push_back(texture);
	
	this->nAttrPerVert = nAttrPerVert;
	this->nVert = nVert;
	this->nTri = nTri;
	this->vertices = vertices;
	this->triangles = triangles;

	this->drawCount = 3 * nTri;

	loadData();
}

Model::Model(Shader * shader, Material * material, vector<Texture*> textures,
	const int nAttrPerVert, const int nVert, const int nTri, const float * vertices, const unsigned int * triangles)
{
	this->shader = shader;
	this->material = material;	
	for (vector<Texture *>::iterator i = textures.begin(); i != textures.end(); i++)
		this->textures.push_back(*i);

	this->nAttrPerVert = nAttrPerVert;
	this->nVert = nVert;
	this->nTri = nTri;
	this->vertices = vertices;
	this->triangles = triangles;

	this->drawCount = 3 * nTri;

	loadData();
}

Model::~Model()
{
}

void Model::loadData()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, nAttrPerVert * nVert * sizeof(float), vertices, GL_STATIC_DRAW);

	GLint posLoc = glGetAttribLocation(shader->shaderProgram, "vPosition");
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, nAttrPerVert * sizeof(float), (void *)0);

	GLint normLoc = glGetAttribLocation(shader->shaderProgram, "vNormal");
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, nAttrPerVert * sizeof(float), (void *)(3 * sizeof(float)));

	GLint texLoc = glGetAttribLocation(shader->shaderProgram, "vTexCoord");
	glEnableVertexAttribArray(texLoc);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, nAttrPerVert * sizeof(float), (void *)(6 * sizeof(float)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * nTri * sizeof(unsigned int), triangles, GL_STATIC_DRAW);

	glBindVertexArray(0);
}
