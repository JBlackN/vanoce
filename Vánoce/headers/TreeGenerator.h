//----------------------------------------------------------------------------------------
/**
* \file       TreeGenerator.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains TreeGenerator class.
*
*  File contains TreeGenerator class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "pgr.h"
#include "Config.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"

/**
* Class provides random tree generation.
*/
class TreeGenerator
{
private:
	Config * config; ///< Config provider.

	/**
	 * Enumeration describes states of one scene map segment.
	 */
	enum MapSegmentStatus : bool
	{
		tree = true, ///< Segment contains a tree.
		treeSurroundings = true, ///< Segment is next to another segment which contains a tree.
		home = true, ///< Segment lies inside home (no tree generation there).
		nothing = false ///< Segment can be used to place a new tree.
	};

	Model * treeModel;
	MapSegmentStatus ** sceneMap; ///< Scene map divided to segments.
	int sceneMapDimension; ///< Number of segments (map edge).
public:
	vector<Object *> trees; ///< Generated and placed trees.
public:
	/**
	 * Creates new tree generator.
	 * @param config Config provider.
	 * @param treeModel Tree model.
	 * @param mapDimension Map QUADRANT edge dimension.
	 */
	TreeGenerator(Config * config, Model * treeModel, int mapDimension);
	~TreeGenerator();

	/**
	 * Generates specified number of trees.
	 * @param count Number of trees to generate.
	 */
	void generateTrees(int count);

	/**
	 * Draws generated trees.
	 * @param camera Camera to use.
	 * @param lights Lights to use.
	 * @param fog Fog to use.
	 * @param firstID First drawn tree's ID (used for later tree picking).
	 */
	void drawTrees(Camera * camera, map<string, Light *> lights, Fog * fog, int firstID);
private:
	/**
	 * Inits segment map of a scene.
	 */
	void initMap();

	/**
	 * Marks segments surrounging placed tree in a scene map.
	 * @param randomX X-coordinate of a placed tree.
	 * @param randomZ Z-coordinate of a placed tree.
	 */
	void markTreeSurroundings(int randomX, int randomZ);
};

