#pragma once

#include <string>
#include <map>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"

class Object
{
public:
	Model * model;
	glm::mat4 placementMatrix;
	glm::mat4 textureMatrix;

	Object(Model * model, glm::mat4 placementMatrix = glm::mat4(1), glm::mat4 textureMatrix = glm::mat4(1));
	~Object();

	void draw(Camera * camera, map<string, Light *> lights = map<string, Light *>(), Fog * fog = NULL);
	void draw(float left, float right, float bottom, float top, float nearPlane, float farPlane);
private:
	void useCamera(Camera * camera);
	void useLights(map<string, Light *> lights);
	void useLight(Light * light, int index);
	void useMaterial();
	void useTextures();
	void useFog(Fog * fog);
};

