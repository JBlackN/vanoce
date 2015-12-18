#pragma once

#include <cstdlib>
#include <string>
#include <list>
#include <map>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"
#include "Spline.h"

class SnowGenerator
{
public:
	struct Snowflake {
		Object * snowflake;
		Spline * path;
		int frame;

		Snowflake(Object * snowflake, Spline * path, int frame)
		{
			this->snowflake = snowflake;
			this->path = path;
			this->frame = frame;
		}
	};
private:
	Model * snowflake;
	list<Snowflake *> snowflakes;

	float fallHeight;
	int snowflakeCount;
	int frameCount;
public:
	SnowGenerator(Model * snowflake, float fallHeight, int snowflakeCount, int secondsToFall, int fps);
	~SnowGenerator();

	void generate();
	void update();
	void checkCount();
	void remove(list<Snowflake *>::iterator i);
	void draw(Camera * camera, map<string, Light *> lights, Fog * fog);
private:
	glm::mat4 getPlacementMatrix(glm::vec3 position);
};

