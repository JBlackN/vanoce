#pragma once

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
public:
	enum OrnamentType { red, yellow, blue };
private:
	Model * redOrnament;
	Model * yellowOrnament;
	Model * blueOrnament;

	unsigned int nRedOrnaments;
	unsigned int nYellowOrnaments;
	unsigned int nBlueOrnaments;

	map<OrnamentType, list<Object *>> placedOrnaments;

	float ornamentDiameter;
	list<glm::vec3> placedPositions;
public:
	Inventory(Model * redOrnament, Model * yellowOrnament, Model * blueOrnament);
	~Inventory();

	void insertOrnament(OrnamentType type, unsigned int count = 1);
	void removeOrnament(OrnamentType type);
	unsigned int ornamentCount(OrnamentType type);
	void placeOrnament(OrnamentType type, Object * tree, glm::vec2 window_dimensions, int windowX, int windowY, Camera * camera);
	void drawOrnaments(Camera * camera, map<string, Light *> lights, Fog * fog);

private:
	glm::vec3 findPosition(Object * tree, glm::vec2 window_dimensions, int windowX, int windowY, Camera * camera);
};

