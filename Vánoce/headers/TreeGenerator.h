#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"

class TreeGenerator
{
public:
	Model * tree;
	vector<Object *> trees;

	TreeGenerator(Model * treeModel);
	~TreeGenerator();

	void generateTrees(int count, int mapQuadrantDimension);
	void drawTrees(Camera * camera, map<string, Light *> lights, Fog * fog, bool useIds = false);
};

