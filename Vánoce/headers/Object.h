#pragma once

#include <string>
#include <map>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Camera.h"
#include "Light.h"

class Object
{
public:
	Model * model;
	glm::mat4 adjustmentMatrix;

	Object(Model * model, glm::mat4 adjustmentMatrix);
	~Object();

	void draw(Camera * camera, map<string, Light *> lights);

private:
	void useCamera(Camera * camera);
	void useLights(map<string, Light *> lights);
	void useLight(Light * light, int index);
	void useMaterial();
};

