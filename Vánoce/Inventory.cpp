#include "headers\Inventory.h"

Inventory::Inventory(Model * redOrnament, Model * yellowOrnament, Model * blueOrnament)
{
	this->redOrnament = redOrnament;
	this->yellowOrnament = yellowOrnament;
	this->blueOrnament = blueOrnament;

	this->nRedOrnaments = 0;
	this->nYellowOrnaments = 0;
	this->nBlueOrnaments = 0;

	//this->placedObjects = list<Object *>();
}

Inventory::~Inventory()
{
}

void Inventory::insertOrnament(OrnamentType type)
{
	switch (type)
	{
	case red:
		this->nRedOrnaments++;
		break;
	case yellow:
		this->nYellowOrnaments++;
		break;
	case blue:
		this->nBlueOrnaments++;
		break;
	}
}

void Inventory::removeOrnament(OrnamentType type)
{
	switch (type)
	{
	case red:
		if (this->nRedOrnaments > 0) this->nRedOrnaments--;
		break;
	case yellow:
		if (this->nYellowOrnaments > 0) this->nYellowOrnaments--;
		break;
	case blue:
		if (this->nBlueOrnaments > 0) this->nBlueOrnaments--;
		break;
	}
}

unsigned int Inventory::ornamentCount(OrnamentType type)
{
	switch (type)
	{
	case red:
		return this->nRedOrnaments;
		break;
	case yellow:
		return this->nYellowOrnaments;
		break;
	case blue:
		return this->nBlueOrnaments;
		break;
	}
}

void Inventory::placeOrnament(OrnamentType type, Object * tree)
{
	if (ornamentCount(type) == 0) return;

	cout << "Placing " << type << endl;
	removeOrnament(type);
}
