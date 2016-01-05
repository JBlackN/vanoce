//----------------------------------------------------------------------------------------
/**
* \file       Hud.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Hud class.
*
*  File contains Hud class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <string>
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

/**
 * Class describes user interface (heads-up display).
 */
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

	Shader * shader;
	map<string, Material *> materials;
	map<string, Texture *> backgrounds;
	vector<Texture *> numbers;
public:
	Hud(int winWidth, int winHeight, Shader * shader, map<string, Material *> materials, map<string, Texture *> textures,
		float hudElementSizePx = 40, float hudElementBorderPercent = 4, bool enabled = false);
	~Hud();

	void reload(int winWidth, int winHeight, float hudElementSizePx = 40, float hudElementBorderPercent = 4,
		bool enabled = false);
	void draw(Inventory * inventory, float left, float right, float bottom, float top,
		float nearPlane = -1.0f, float farPlane = 100.0f);
private:
	/**
	 * Gets correct texture based on number of ornaments (of specified color) in inventory.
	 * @param inventory Inventory.
	 * @param type Ornament color (red, yellow, blue).
	 * @return Texture.
	 */
	Texture * getNumberTexture(Inventory * inventory, Inventory::OrnamentType type);
};

