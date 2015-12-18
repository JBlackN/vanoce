#include "headers\SnowGenerator.h"

SnowGenerator::SnowGenerator(Model * snowflake, float fallHeight, int snowflakeCount, int secondsToFall, int fps)
{
	this->snowflake = snowflake;
	this->fallHeight = fallHeight;
	this->snowflakeCount = snowflakeCount;
	this->frameCount = secondsToFall * fps;

	for (int i = 0; i < snowflakeCount; i++) generate();
}

SnowGenerator::~SnowGenerator()
{
}

void SnowGenerator::generate()
{
	glm::vec3 start_position = glm::vec3(rand() % 38 - 19, fallHeight, rand() % 38 - 19);

	Object * newSnowflake = new Object(snowflake, getPlacementMatrix(start_position));
	Spline * newPath = new Spline(start_position, 4, 0); // TODO: Angle

	snowflakes.push_back(new Snowflake(newSnowflake, newPath, 0));
}

void SnowGenerator::update()
{
	list<Snowflake *>::iterator i = snowflakes.begin();
	while (i != snowflakes.end())
	{
		(*i)->snowflake->adjustmentMatrix = getPlacementMatrix((*i)->path->point(((*i)->frame)++, frameCount)->position);
		if (((*i)->frame) >= frameCount)
		{
			remove(i++);
			continue;
		}
		else i++;
	}
}

void SnowGenerator::checkCount()
{
	while (snowflakes.size() < snowflakeCount) generate();
}

void SnowGenerator::remove(list<Snowflake*>::iterator i)
{
	snowflakes.erase(i);
	generate();
}

void SnowGenerator::draw(Camera * camera, map<string, Light *> lights, Fog * fog)
{
	for (list<Snowflake *>::iterator i = snowflakes.begin(); i != snowflakes.end(); i++)
		(*i)->snowflake->draw(camera, lights, fog);
}

glm::mat4 SnowGenerator::getPlacementMatrix(glm::vec3 position)
{
	return glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)), position);
}
