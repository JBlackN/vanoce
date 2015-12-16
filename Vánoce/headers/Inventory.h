#pragma once

#include <iostream> // TODO: Remove
#include <string>
#include <map>
#include <list>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Fog.h"

class Inventory
{
private:
	Model * redOrnament;
	Model * yellowOrnament;
	Model * blueOrnament;

	unsigned int nRedOrnaments;
	unsigned int nYellowOrnaments;
	unsigned int nBlueOrnaments;

	list<Object *> placedOrnaments;

public:
	enum OrnamentType { red, yellow, blue };

	Inventory(Model * redOrnament, Model * yellowOrnament, Model * blueOrnament);
	~Inventory();

	void insertOrnament(OrnamentType type);
	void removeOrnament(OrnamentType type);
	unsigned int ornamentCount(OrnamentType type);
	void placeOrnament(OrnamentType type, Object * tree, glm::vec2 window_dimensions, int windowX, int windowY, Camera * camera);
	void drawOrnaments(Camera * camera, map<string, Light *> lights, Fog * fog);

private:
	glm::vec3 findPosition(Object * tree, glm::vec2 window_dimensions, int windowX, int windowY, Camera * camera);
};

