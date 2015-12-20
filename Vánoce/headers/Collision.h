#pragma once

#include <vector>
using namespace std;

#include "pgr.h"
#include "Object.h"

class Collision
{
private:
	glm::vec3 boundaries_min;
	glm::vec3 boundaries_max;
public:
	Collision(Object * object);
	~Collision();

	bool check(glm::vec3 world_position);
};

