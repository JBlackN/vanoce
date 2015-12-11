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

	useCamera(camera);
	useLight(light);
	useMaterial();

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

void Object::useCamera(Camera * camera)
{
	GLint projLoc = glGetUniformLocation(model->shader->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	GLint viewLoc = glGetUniformLocation(model->shader->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
}

void Object::useLight(Light * light)
{
	// Position

	GLint lightPosLoc = glGetUniformLocation(model->shader->shaderProgram, "light.position");
	glUniform3f(lightPosLoc, light->position.x, light->position.y, light->position.z);

	// Colors

	GLint lightAmbientLoc = glGetUniformLocation(model->shader->shaderProgram, "light.ambient");
	glUniform3f(lightAmbientLoc, light->ambient.x, light->ambient.y, light->ambient.z);

	GLint lightDiffuseLoc = glGetUniformLocation(model->shader->shaderProgram, "light.diffuse");
	glUniform3f(lightDiffuseLoc, light->diffuse.x, light->diffuse.y, light->diffuse.z);

	GLint lightSpecularLoc = glGetUniformLocation(model->shader->shaderProgram, "light.specular");
	glUniform3f(lightSpecularLoc, light->specular.x, light->specular.y, light->specular.z);

	// Spotlight parameters

	GLint lightSpotDirLoc = glGetUniformLocation(model->shader->shaderProgram, "light.spot_direction");
	glUniform3f(lightSpotDirLoc, light->spot_direction.x, light->spot_direction.y, light->spot_direction.z);

	GLint lightSpotExpLoc = glGetUniformLocation(model->shader->shaderProgram, "light.spot_exponent");
	glUniform1f(lightSpotExpLoc, light->spot_exponent);

	GLint lightSpotCutLoc = glGetUniformLocation(model->shader->shaderProgram, "light.spot_cutoff");
	glUniform1f(lightSpotCutLoc, light->spot_cutoff);

	// Attenuation parameters

	GLint lightConstAttLoc = glGetUniformLocation(model->shader->shaderProgram, "light.constant_attenuation");
	glUniform1f(lightConstAttLoc, light->constant_attenuation);

	GLint lightLinAttLoc = glGetUniformLocation(model->shader->shaderProgram, "light.linear_attenuation");
	glUniform1f(lightLinAttLoc, light->linear_attenuation);

	GLint lightQuadAttLoc = glGetUniformLocation(model->shader->shaderProgram, "light.quadratic_attenuation");
	glUniform1f(lightQuadAttLoc, light->quadratic_attenuation);
}

void Object::useMaterial()
{
	GLint matAmbientLoc = glGetUniformLocation(model->shader->shaderProgram, "material.ambient");
	glUniform3f(matAmbientLoc, model->material->ambient.x, model->material->ambient.y, model->material->ambient.z);

	GLint matDiffuseLoc = glGetUniformLocation(model->shader->shaderProgram, "material.diffuse");
	glUniform3f(matDiffuseLoc, model->material->diffuse.x, model->material->diffuse.y, model->material->diffuse.z);

	GLint matSpecularLoc = glGetUniformLocation(model->shader->shaderProgram, "material.specular");
	glUniform3f(matSpecularLoc, model->material->specular.x, model->material->specular.y, model->material->specular.z);

	GLint matEmissionLoc = glGetUniformLocation(model->shader->shaderProgram, "material.emission");
	glUniform3f(matEmissionLoc, model->material->emission.x, model->material->emission.y, model->material->emission.z);

	GLint matShininessLoc = glGetUniformLocation(model->shader->shaderProgram, "material.shininess");
	glUniform1f(matShininessLoc, model->material->shininess);
}
