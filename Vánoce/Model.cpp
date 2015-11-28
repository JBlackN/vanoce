#include "headers\Model.h"

Model::Model(Shader * shader, Texture * texture,
	const int nAttrPerVert, const int nVert, const int nTri, const float * vertArray, const unsigned * triArray)
{
	this->shader = shader;
	this->texture = texture;
	this->drawCount = 3 * nTri;

	loadData(nAttrPerVert, nVert, nTri, vertArray, triArray);
}

Model::~Model()
{
}

void Model::loadData(const int nAttrPerVert, const int nVert, const int nTri, const float * vertArray, const unsigned * triArray)
{
	glUseProgram(shader->getProgram());

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertArray), vertArray, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triArray), triArray, GL_STATIC_DRAW);

	GLint positionLoc = glGetAttribLocation(shader->getProgram(), "position");
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, nAttrPerVert * sizeof(float), NULL);

	GLint normLoc = glGetAttribLocation(shader->getProgram(), "norm");
	glEnableVertexAttribArray(normLoc);
	glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, nAttrPerVert * sizeof(float), (void *)(3 * sizeof(float)));

	GLint vTexCoordLoc = glGetAttribLocation(shader->getProgram(), "vTexCoord");
	glEnableVertexAttribArray(vTexCoordLoc);
	glVertexAttribPointer(vTexCoordLoc, 3, GL_FLOAT, GL_FALSE, nAttrPerVert * sizeof(float), (void *)(6 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

Shader * Model::getShader()
{
	return this->shader;
}

Texture * Model::getTexture()
{
	return this->texture;
}

GLuint Model::getVAO()
{
	return this->vao;
}

GLint Model::getDrawCount()
{
	return this->drawCount;
}
