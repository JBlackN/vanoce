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
private:
	enum MapSegmentStatus : bool { tree = true, treeSurroundings = true, home = true, nothing = false };

	Model * treeModel;
	MapSegmentStatus ** sceneMap;
	int sceneMapDimension;
public:
	vector<Object *> trees;
public:
	TreeGenerator(Model * treeModel, int mapDimension);
	~TreeGenerator();

	void generateTrees(int count);
	void drawTrees(Camera * camera, map<string, Light *> lights, Fog * fog, int firstID);
private:
	void initMap();
	void markTreeSurroundings(int randomX, int randomZ);
};

