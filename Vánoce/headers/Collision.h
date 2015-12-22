#pragma once

#include <vector>
#include <list>
using namespace std;

#include "pgr.h"
#include "Config.h"
#include "Object.h"

class Collision
{
private:
	Config * config;

	glm::vec3 homeBoundariesMin;
	glm::vec3 homeBoundariesMax;

	float ornamentDiameter;
	list<glm::vec3> ornamentPositions;
public:
	Collision(Config * config, Object * object);
	Collision(Config * config, int nAttrPerVertex, int nVert, const float * vertices);
	~Collision();

	bool check(glm::vec3 world_position);
	bool sphereCheck(glm::vec3 position);
	void addOrnamentPosition(glm::vec3 position);
};

