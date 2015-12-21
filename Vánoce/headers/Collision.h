#pragma once

#include <vector>
#include <list>
using namespace std;

#include "pgr.h"
#include "Object.h"

class Collision
{
private:
	glm::vec3 homeBoundariesMin;
	glm::vec3 homeBoundariesMax;

	float ornamentDiameter;
	list<glm::vec3> ornamentPositions;
public:
	Collision(Object * object);
	Collision(int nAttrPerVertex, int nVert, const float * vertices);
	~Collision();

	bool check(glm::vec3 world_position);
	bool sphereCheck(glm::vec3 position);
	void addOrnamentPosition(glm::vec3 position);
};

