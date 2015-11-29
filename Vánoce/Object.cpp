#include "headers\Object.h"

Object::Object(Model * model, glm::mat4 adjustmentMatrix)
{
	this->model = model;
	this->adjustmentMatrix = adjustmentMatrix;
}

Object::~Object()
{
}

void Object::drawObject()
{
	GLuint shaderProgram = this->model->getShader()->getProgram();
	glUseProgram(shaderProgram);

	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->adjustmentMatrix));

	glBindVertexArray(this->model->getVAO());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->model->getTexture()->getTexture());

	GLint texLoc = glGetUniformLocation(shaderProgram, "tex");
	glUniform1i(texLoc, 0);

	glDrawElements(GL_TRIANGLES, this->model->getDrawCount(), GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}
