#include "headers\Texture.h"

Texture::Texture(string texPath)
{
	texture = pgr::createTexture(texPath);
}

Texture::~Texture()
{
}