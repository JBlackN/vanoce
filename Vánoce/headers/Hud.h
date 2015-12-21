#pragma once

#include <vector>
#include <map>
using namespace std;

#include "pgr.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Model.h"
#include "Object.h"
#include "Inventory.h"

class Hud
{
public:
	bool enabled;
	float hudElementSizePx;
	float hudElementBorderPercent;
private:
	float * hudElementVertices;
	unsigned int * hudElementTriangles;

	Object * hudOrnamentRed;
	Object * hudOrnamentYellow;
	Object * hudOrnamentBlue;

	vector<Texture *> numbers;
public:
	Hud(int winWidth, int winHeight, Shader * shader, map<string, Material *> materials, map<string, Texture *> textures,
		float hudElementSizePx = 40, float hudElementBorderPercent = 4, bool enabled = false);
	~Hud();

	void draw(Inventory * inventory, float left, float right, float bottom, float top,
		float nearPlane = -1.0f, float farPlane = 100.0f);
private:
	Texture * getNumberTexture(Inventory * inventory, Inventory::OrnamentType type);
};

