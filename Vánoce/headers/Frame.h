#pragma once

#include "pgr.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"

class Frame
{
public:
	int fps;
	float seconds;
private:
	Object * frame;
	glm::mat4 textureAdjustmentMatrix;
	int frameCounter;
	int currentPicture;
public:
	Frame(Model * frame, glm::mat4 matrix, int fps = 25, float seconds = 5.0f);
	~Frame();

	void draw(Camera * camera, map<string, Light *> lights, Fog * fog);
	void update();
};

