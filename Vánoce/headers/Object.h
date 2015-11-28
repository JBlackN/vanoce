#pragma once

#include "pgr.h"
#include "headers/Model.h"

class Object
{
public:
	Object(Model *, glm::mat4);
	~Object();
private:
	Model * model;
	glm::mat4 adjustmentMatrix;
public:
	void drawObject();
};

