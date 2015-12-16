#pragma once

#include <iostream> // TODO: Remove
#include <list>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Object.h"

class Inventory
{
private:
	Model * redOrnament;
	Model * yellowOrnament;
	Model * blueOrnament;

	unsigned int nRedOrnaments;
	unsigned int nYellowOrnaments;
	unsigned int nBlueOrnaments;

	list<Object *> placedObjects;

public:
	enum OrnamentType { red, yellow, blue };

	Inventory(Model * redOrnament, Model * yellowOrnament, Model * blueOrnament);
	~Inventory();

	void insertOrnament(OrnamentType type);
	void removeOrnament(OrnamentType type);
	unsigned int ornamentCount(OrnamentType type);
	void placeOrnament(OrnamentType type, Object * tree);
};

