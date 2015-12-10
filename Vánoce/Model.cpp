#include "headers\Model.h"

Model::Model(Shader * shader, Texture * texture, const int nVertices)
{
	this->shader = shader;
	this->texture = texture;
	this->nVertices = nVertices;
}

Model::~Model()
{
}

void Model::loadData(const int nAttrPerVert, const int nVert, const int nTri, const float * vertices, const unsigned int * triangles)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, nAttrPerVert * nVert * sizeof(float), vertices, GL_STATIC_DRAW);

	GLint posLoc = glGetAttribLocation(shader->shaderProgram, "position");
	glEnableVertexAttribArray(posLoc);
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, nAttrPerVert * sizeof(float), (void *)0);

	GLint texLoc = glGetAttribLocation(shader->shaderProgram, "vTexCoord");
	glEnableVertexAttribArray(texLoc);
	glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, nAttrPerVert * sizeof(float), (void *)(6 * sizeof(float)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nVert * sizeof(unsigned int), triangles, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
