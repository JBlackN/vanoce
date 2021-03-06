//----------------------------------------------------------------------------------------
/**
* \file       Texture.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Texture class.
*
*  File contains Texture class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <iostream>
#include <string>
using namespace std;

#include "pgr.h"

/**
* Class holds one texture.
*/
class Texture
{
public:
	GLuint texture;
public:
	/**
	 * Loads new texture using specified path. Clamps texture to edge (both directions) if requested.
	 * @param texPath Path to image file.
	 * @param clamp Specifies whether clamp texture to edges or not.
	 */
	Texture(string texPath, bool clamp = false);
	~Texture();
};

