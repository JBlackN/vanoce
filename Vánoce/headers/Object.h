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
	glm::mat4 adjustmentMatrix;

	Object(Model * model, glm::mat4 adjustmentMatrix);
	~Object();

	void draw(Camera * camera, map<string, Light *> lights = map<string, Light *>(), Fog * fog = NULL, glm::mat4 textureAdjustmentMatrix = glm::mat4(1));
	void draw(Texture * numberTexture, float left, float right, float bottom, float top, float nearPlane, float farPlane,
		glm::mat4 textureAdjustmentMatrix = glm::mat4(1));

private:
	void useCamera(Camera * camera);
	void useLights(map<string, Light *> lights);
	void useLight(Light * light, int index);
	void useMaterial();
	void useFog(Fog * fog);
};

