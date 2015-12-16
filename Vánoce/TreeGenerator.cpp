#include "headers\TreeGenerator.h"

TreeGenerator::TreeGenerator(Model * treeModel)
{
	tree = treeModel;
	srand(time(NULL));
}

TreeGenerator::~TreeGenerator()
{
}

void TreeGenerator::generateTrees(int count, int mapDimension)
{
	mapDimension = 2 * mapDimension + 1;

	bool ** map = new bool*[mapDimension];
	for (int i = 0; i < mapDimension; i++)
		map[i] = new bool[mapDimension];

	for (int i = 0; i < mapDimension; i++)
	{
		for (int j = 0; j < mapDimension; j++)
		{
			int k = i - (mapDimension - 1) / 2;
			int l = j - (mapDimension - 1) / 2;

			if (k >= -5 && k <= 5 && l >= -5 && l <= 5)
				map[i][j] = true;
			else
				map[i][j] = false;
		}
	}

	for (int i = 0; i < count; i++)
	{
		int randomX = rand() % (mapDimension - 1);
		int randomZ = rand() % (mapDimension - 1);

		if (!map[randomX][randomZ])
		{
			map[randomX][randomZ] = true;

			if (randomX + 1 < mapDimension && randomZ + 1 < mapDimension) map[randomX + 1][randomZ + 1] = true;
			if (randomZ + 1 < mapDimension) map[randomX][randomZ + 1] = true;
			if (randomX - 1 >= 0 && randomZ + 1 < mapDimension) map[randomX - 1][randomZ + 1] = true;
			if (randomX + 1 < mapDimension) map[randomX + 1][randomZ] = true;
			if (randomX - 1 >= 0) map[randomX - 1][randomZ] = true;
			if (randomX + 1 < mapDimension && randomZ - 1 >= 0) map[randomX + 1][randomZ - 1] = true;
			if (randomZ - 1 >= 0) map[randomX][randomZ - 1] = true;
			if (randomX - 1 >= 0 && randomZ - 1 >= 0) map[randomX - 1][randomZ - 1] = true;

			Object * newTree = new Object(tree, glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
				glm::vec3(randomX - (mapDimension - 1) / 2, 1.2f, randomZ - (mapDimension - 1) / 2)));
			trees.push_back(newTree);
		}
		else
		{
			bool guard = true;
			for (int i = 0; i < mapDimension; i++)
				for (int j = 0; j < mapDimension; j++)
					if (map[i][j] == false) guard = false;

			if (!guard)
			{
				i--;
				continue;
			}
			else
				break;
		}
	}

	for (int i = 0; i < mapDimension; i++)
		delete map[i];
	delete map;
}

void TreeGenerator::drawTrees(Camera * camera, map<string, Light *> lights, Fog * fog, bool useIds)
{
	int id = 5;
	for (vector<Object *>::iterator i = trees.begin(); i != trees.end(); i++)
	{
		if (useIds) glStencilFunc(GL_ALWAYS, id++, -1);
		(*i)->draw(camera, lights, fog);
	}
}
