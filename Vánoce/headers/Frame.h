#pragma once

#include "pgr.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"

class Frame
{
public:
	int fps; ///< Frames per second.
	float seconds; ///< How many seconds is one image displayed.
private:
	Object * frame;
	int frameCounter; ///< Current frame.
	int currentPicture;
public:
	/**
	 * Creates new picture frame.
	 * @param frame Frame model.
	 * @param matrix Frame placement matrix.
	 * @param fps Frames per second.
	 * @param seconds How many seconds is one image displayed.
	 */
	Frame(Model * frame, glm::mat4 matrix, int fps = 25, float seconds = 5.0f);
	~Frame();

	void draw(Camera * camera, map<string, Light *> lights, Fog * fog);

	/**
	 * Updates picture frame based on current animation frame.
	 */
	void update();
};

