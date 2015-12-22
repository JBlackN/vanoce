#include "headers\TreeGenerator.h"

TreeGenerator::TreeGenerator(Config * config, Model * treeModel, int mapDimension)
{
	this->config = config;
	this->treeModel = treeModel;
	this->sceneMapDimension = mapDimension;
	initMap();
}

TreeGenerator::~TreeGenerator()
{
	trees.clear();
	for (int i = 0; i < sceneMapDimension; i++)
		delete sceneMap[i];
	delete sceneMap;
}

void TreeGenerator::generateTrees(int count)
{
	for (int i = 0; i < count; i++)
	{
		int randomX = rand() % (sceneMapDimension - 1);
		int randomZ = rand() % (sceneMapDimension - 1);

		if (sceneMap[randomX][randomZ] == nothing)
		{
			sceneMap[randomX][randomZ] = tree;
			markTreeSurroundings(randomX, randomZ);

			Object * newTree = new Object(treeModel, glm::translate(glm::scale(glm::mat4(1),
				config->fOpt("scale") * glm::vec3(5, 5, 5)),
				glm::vec3(randomX - (sceneMapDimension - 1) / 2, 1.2f, randomZ - (sceneMapDimension - 1) / 2)));
			trees.push_back(newTree);
		}
		else // Check if map is full
		{
			bool guard = true;
			for (int i = 0; i < sceneMapDimension; i++)
				for (int j = 0; j < sceneMapDimension; j++)
					if (sceneMap[i][j] == nothing) guard = false;

			if (!guard)
			{
				i--;
				continue;
			}
			else
				break;
		}
	}
}

void TreeGenerator::drawTrees(Camera * camera, map<string, Light *> lights, Fog * fog, int firstID)
{
	for (vector<Object *>::iterator i = trees.begin(); i != trees.end(); i++)
	{
		if (firstID > 0) glStencilFunc(GL_ALWAYS, firstID++, -1);
		(*i)->draw(camera, lights, fog);
	}
}

void TreeGenerator::initMap()
{
	sceneMapDimension = 2 * sceneMapDimension + 1;

	sceneMap = new MapSegmentStatus*[sceneMapDimension];
	for (int i = 0; i < sceneMapDimension; i++)
		sceneMap[i] = new MapSegmentStatus[sceneMapDimension];

	for (int i = 0; i < sceneMapDimension; i++)
	{
		for (int j = 0; j < sceneMapDimension; j++)
		{
			int k = i - (sceneMapDimension - 1) / 2;
			int l = j - (sceneMapDimension - 1) / 2;

			float min_xz = -5 * config->fOpt("scale");
			float max_xz = 5 * config->fOpt("scale");

			if (k >= min_xz && k <= max_xz && l >= min_xz && l <= max_xz)
				sceneMap[i][j] = home;
			else
				sceneMap[i][j] = nothing;
		}
	}
}

void TreeGenerator::markTreeSurroundings(int randomX, int randomZ)
{
	if (randomX + 1 < sceneMapDimension && randomZ + 1 < sceneMapDimension)
		sceneMap[randomX + 1][randomZ + 1] = treeSurroundings;
	if (randomZ + 1 < sceneMapDimension) sceneMap[randomX][randomZ + 1] = treeSurroundings;
	if (randomX - 1 >= 0 && randomZ + 1 < sceneMapDimension) sceneMap[randomX - 1][randomZ + 1] = treeSurroundings;
	if (randomX + 1 < sceneMapDimension) sceneMap[randomX + 1][randomZ] = treeSurroundings;
	if (randomX - 1 >= 0) sceneMap[randomX - 1][randomZ] = treeSurroundings;
	if (randomX + 1 < sceneMapDimension && randomZ - 1 >= 0) sceneMap[randomX + 1][randomZ - 1] = treeSurroundings;
	if (randomZ - 1 >= 0) sceneMap[randomX][randomZ - 1] = treeSurroundings;
	if (randomX - 1 >= 0 && randomZ - 1 >= 0) sceneMap[randomX - 1][randomZ - 1] = treeSurroundings;
}
