//----------------------------------------------------------------------------------------
/**
* \file       Overlay.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Overlay class.
*
*  File contains Overlay class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <map>
using namespace std;

#include "pgr.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Model.h"
#include "Object.h"

/**
 * Class describes animated overlay projected over entire scene.
 */
class Overlay
{
public:
	bool enabled;
	int fps; ///< Frames per second.
	float seconds; ///< Duration of one animation cycle in seconds.
private:
	float * overlayVertices;
	unsigned int * overlayTriangles;

	Object * overlay;

	float magnification; ///< Amount of scale applied during animation.
	float offset; ///< Current position of animation.
	float scale; ///< Current scale of animation.
	enum ScaleDirection {ascending, descending} scaleDirection; ///< Describes whether scale is ascending or descending.
public:
	Overlay(int winWidth, int winHeight, Shader * shader, Material * material, map<string, Texture *> textures,
		bool enabled = false, int fps = 25, float seconds = 5.0f, float magnification = 25.0f);
	~Overlay();

	/**
	* Draws an overlay using orthographic projection.
	* @param left Left clipping plane.
	* @param right Right clipping plane.
	* @param bottom Bottom clipping plane.
	* @param top Top clipping plane.
	* @param nearPlane Near clipping plane.
	* @param farPlane Far clipping plane.
	*/
	void draw(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 100.0f);

	/**
	 * Updates overlay animation.
	 */
	void update();
};
