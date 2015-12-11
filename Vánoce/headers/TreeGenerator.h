#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"

class TreeGenerator
{
public:
	Model * tree;
	vector<Object *> trees;

	TreeGenerator(Model * treeModel);
	~TreeGenerator();

	void generateTrees(int count, int mapQuadrantDimension);
	void drawTrees(Camera * camera);
};

