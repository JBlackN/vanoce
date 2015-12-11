#pragma once

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

	void draw(Camera * camera, Light * light);
};

