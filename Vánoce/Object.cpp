#include "headers\Object.h"

Object::Object(Model * model, glm::mat4 adjustmentMatrix)
{
	this->model = model;
	this->adjustmentMatrix = adjustmentMatrix;
}

Object::~Object()
{
}

void Object::draw(Camera * camera, Light * light)
{
	glUseProgram(model->shader->shaderProgram);
	glBindVertexArray(model->vao);

	GLint modelLoc = glGetUniformLocation(model->shader->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->adjustmentMatrix));

	GLint projLoc = glGetUniformLocation(model->shader->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	GLint viewLoc = glGetUniformLocation(model->shader->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

	GLint lightPosLoc = glGetUniformLocation(model->shader->shaderProgram, "light.position");
	glUniform3f(lightPosLoc, light->position.x, light->position.y, light->position.z);
	
	GLint lightColorLoc = glGetUniformLocation(model->shader->shaderProgram, "light.color");
	glUniform3f(lightColorLoc, light->color.x, light->color.y, light->color.z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->texture->texture);
	GLint texLoc = glGetUniformLocation(model->shader->shaderProgram, "tex");
	glUniform1i(texLoc, 0);

	glDrawElements(GL_TRIANGLES, model->drawCount, GL_UNSIGNED_INT, (void *)0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	glBindVertexArray(0);
	glUseProgram(0);
}
