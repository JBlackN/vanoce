#include "headers\Shader.h"
#include "headers\Texture.h"

Texture::Texture(Shader * shader, string texPath)
{
	//glUseProgram(shader->getProgram());

	texture = pgr::createTexture(texPath);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	//glUseProgram(0);
}

Texture::~Texture()
{
}

GLuint Texture::getTexture()
{
	return texture;
}
