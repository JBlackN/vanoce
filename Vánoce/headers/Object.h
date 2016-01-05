//----------------------------------------------------------------------------------------
/**
* \file       Object.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Object class.
*
*  File contains Object class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <map>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"

/**
 * Class describes an instance of model placed within the scene.
 */
class Object
{
public:
	Model * model;
	glm::mat4 placementMatrix;
	glm::mat4 textureMatrix; ///< Used for texture coordinates adjustments.

	Object(Model * model, glm::mat4 placementMatrix = glm::mat4(1), glm::mat4 textureMatrix = glm::mat4(1));
	~Object();

	/**
	 * Draws an object using specified camera.
	 * @param camera Active camera.
	 * @param lights Lights.
	 * @param for Fog.
	 */
	void draw(Camera * camera, map<string, Light *> lights = map<string, Light *>(), Fog * fog = NULL);

	/**
	* Draws an object using orthographic projection.
	* @param left Left clipping plane.
	* @param right Right clipping plane.
	* @param bottom Bottom clipping plane.
	* @param top Top clipping plane.
	* @param nearPlane Near clipping plane.
	* @param farPlane Far clipping plane.
	*/
	void draw(float left, float right, float bottom, float top, float nearPlane, float farPlane);
private:
	void useCamera(Camera * camera);
	void useLights(map<string, Light *> lights);
	void useLight(Light * light, int index);
	void useMaterial();
	void useTextures();
	void useFog(Fog * fog);
};

