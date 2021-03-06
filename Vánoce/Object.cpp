#include "headers\Object.h"

Object::Object(Model * model, glm::mat4 placementMatrix, glm::mat4 textureMatrix)
{
	this->model = model;
	this->placementMatrix = placementMatrix;
	this->textureMatrix = textureMatrix;
}

Object::~Object()
{
}

void Object::draw(Camera * camera, map<string, Light *> lights, Fog * fog)
{
	glUseProgram(model->shader->shaderProgram);
	glBindVertexArray(model->vao);

	GLint modelLoc = glGetUniformLocation(model->shader->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(placementMatrix));

	useCamera(camera);
	useLights(lights);
	useMaterial();
	useTextures();
	if (fog != NULL) useFog(fog);

	glDrawElements(GL_TRIANGLES, model->drawCount, GL_UNSIGNED_INT, (void *)0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Object::draw(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	glUseProgram(model->shader->shaderProgram);
	glBindVertexArray(model->vao);

	GLint modelLoc = glGetUniformLocation(model->shader->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(placementMatrix));

	GLint projLoc = glGetUniformLocation(model->shader->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(glm::ortho(left, right, bottom, top, nearPlane, farPlane)));

	GLint viewLoc = glGetUniformLocation(model->shader->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));

	useMaterial();
	useTextures();

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

	GLint camPosLoc = glGetUniformLocation(model->shader->shaderProgram, "cameraPosition");
	glUniformMatrix4fv(camPosLoc, 1, GL_FALSE, glm::value_ptr(camera->position));
}

void Object::useLights(map<string, Light *> lights)
{
	int index = 0;
	for (map<string, Light *>::iterator i = lights.begin(); i != lights.end(); i++)
		useLight(i->second, index++);

	// Light count

	GLint lightCountLoc = glGetUniformLocation(model->shader->shaderProgram, "lightCount");
	glUniform1i(lightCountLoc, lights.size());
}

void Object::useLight(Light * light, int index)
{
	// Uniform names

	string lightBaseUName = "lights[" + to_string(index) + "]";

	string lightTypeUName = lightBaseUName + ".type";
	string lightEnabledUName = lightBaseUName + ".enabled";
	string lightPosUName = lightBaseUName + ".position";
	string lightAmbientUName = lightBaseUName + ".ambient";
	string lightDiffuseUName = lightBaseUName + ".diffuse";
	string lightSpecularUName = lightBaseUName + ".specular";
	string lightSpotDirUName = lightBaseUName + ".spotDirection";
	string lightSpotExpUName = lightBaseUName + ".spotExponent";
	string lightSpotCutUName = lightBaseUName + ".spotCutoff";
	string lightConstAttUName = lightBaseUName + ".constantAttenuation";
	string lightLinAttUName = lightBaseUName + ".linearAttenuation";
	string lightQuadAttUName = lightBaseUName + ".quadraticAttenuation";

	// Type

	GLint lightTypeLoc = glGetUniformLocation(model->shader->shaderProgram, lightTypeUName.c_str());
	glUniform1i(lightTypeLoc, light->type);

	// Enabled

	GLint lightEnabledLoc = glGetUniformLocation(model->shader->shaderProgram, lightEnabledUName.c_str());
	glUniform1i(lightEnabledLoc, light->enabled);

	// Position

	GLint lightPosLoc = glGetUniformLocation(model->shader->shaderProgram, lightPosUName.c_str());
	glUniform3f(lightPosLoc, light->position.x, light->position.y, light->position.z);

	// Colors

	GLint lightAmbientLoc = glGetUniformLocation(model->shader->shaderProgram, lightAmbientUName.c_str());
	glUniform3f(lightAmbientLoc, light->ambient.x, light->ambient.y, light->ambient.z);

	GLint lightDiffuseLoc = glGetUniformLocation(model->shader->shaderProgram, lightDiffuseUName.c_str());
	glUniform3f(lightDiffuseLoc, light->diffuse.x, light->diffuse.y, light->diffuse.z);

	GLint lightSpecularLoc = glGetUniformLocation(model->shader->shaderProgram, lightSpecularUName.c_str());
	glUniform3f(lightSpecularLoc, light->specular.x, light->specular.y, light->specular.z);

	// Spotlight parameters

	GLint lightSpotDirLoc = glGetUniformLocation(model->shader->shaderProgram, lightSpotDirUName.c_str());
	glUniform3f(lightSpotDirLoc, light->spot_direction.x, light->spot_direction.y, light->spot_direction.z);

	GLint lightSpotExpLoc = glGetUniformLocation(model->shader->shaderProgram, lightSpotExpUName.c_str());
	glUniform1f(lightSpotExpLoc, light->spot_exponent);

	GLint lightSpotCutLoc = glGetUniformLocation(model->shader->shaderProgram, lightSpotCutUName.c_str());
	glUniform1f(lightSpotCutLoc, light->spot_cutoff);

	// Attenuation parameters

	GLint lightConstAttLoc = glGetUniformLocation(model->shader->shaderProgram, lightConstAttUName.c_str());
	glUniform1f(lightConstAttLoc, light->constant_attenuation);

	GLint lightLinAttLoc = glGetUniformLocation(model->shader->shaderProgram, lightLinAttUName.c_str());
	glUniform1f(lightLinAttLoc, light->linear_attenuation);

	GLint lightQuadAttLoc = glGetUniformLocation(model->shader->shaderProgram, lightQuadAttUName.c_str());
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

void Object::useTextures()
{
	GLint textureMatrixLoc = glGetUniformLocation(model->shader->shaderProgram, "textureAdjustmentMatrix");
	if (textureMatrixLoc != -1) glUniformMatrix4fv(textureMatrixLoc, 1, GL_FALSE, glm::value_ptr(textureMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model->textures[0]->texture);
	GLint bgTexLoc = glGetUniformLocation(model->shader->shaderProgram, "bgTex");
	glUniform1i(bgTexLoc, 0);

	if ((model->textures).size() > 1)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, model->textures[1]->texture);
		GLint fgTexLoc = glGetUniformLocation(model->shader->shaderProgram, "fgTex");
		glUniform1i(fgTexLoc, 1);
	}
}

void Object::useFog(Fog * fog)
{
	GLint fogDensityLoc = glGetUniformLocation(model->shader->shaderProgram, "fog.density");
	glUniform1f(fogDensityLoc, fog->density);

	GLint fogColorLoc = glGetUniformLocation(model->shader->shaderProgram, "fog.color");
	glUniform4f(fogColorLoc, fog->color.r, fog->color.g, fog->color.b, fog->color.a);

	GLint fogEnabledLoc = glGetUniformLocation(model->shader->shaderProgram, "fog.enabled");
	glUniform1i(fogEnabledLoc, fog->enabled);
}
