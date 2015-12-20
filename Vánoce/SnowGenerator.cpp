#include "headers\SnowGenerator.h"

SnowGenerator::SnowGenerator(Model * snowflake, float fallHeight, int snowflakeCount, int secondsToFall, int fps, bool enabled)
{
	this->enabled = enabled;
	this->snowflake = snowflake;
	this->fallHeight = fallHeight;
	this->snowflakeCount = snowflakeCount;
	this->frameCount = secondsToFall * fps;

	for (int i = 0; i < snowflakeCount; i++) generate();
}

SnowGenerator::~SnowGenerator()
{
}

void SnowGenerator::enable()
{
	this->enabled = true;
	for (int i = 0; i < snowflakeCount; i++) generate();
}

void SnowGenerator::disable()
{
	this->enabled = false;
	snowflakes.clear();
}

bool SnowGenerator::isEnabled()
{
	return this->enabled;
}

void SnowGenerator::generate()
{
	if (!enabled) return;

	glm::vec3 start_position = glm::vec3(rand() % 38 - 19, fallHeight, rand() % 38 - 19);

	Object * newSnowflake = new Object(snowflake, glm::translate(glm::scale(glm::mat4(1), glm::vec3(5, 5, 5)), start_position));
	Spline * newPath = new Spline(start_position, 4, 0); // TODO: Angle

	snowflakes.push_back(new Snowflake(newSnowflake, newPath, 0));
}

void SnowGenerator::update(Collision * collision)
{
	if (!enabled) return;

	list<Snowflake *>::iterator i = snowflakes.begin();
	while (i != snowflakes.end())
	{
		(*i)->snowflake->placementMatrix = getPlacementMatrix(
			(*i),
			(*i)->path->point((*i)->frame, frameCount)->position,
			(*i)->path->point((*i)->frame, frameCount)->direction);
		((*i)->frame)++;

		if ((((*i)->frame) >= frameCount) || collision->check((*i)->path->point((*i)->frame - 1, frameCount)->position))
		{
			remove(i++);
			continue;
		}
		else i++;
	}
}

void SnowGenerator::remove(list<Snowflake*>::iterator i)
{
	if (!enabled) return;

	snowflakes.erase(i);
	generate();
}

void SnowGenerator::draw(Camera * camera, map<string, Light *> lights, Fog * fog)
{
	if (!enabled) return;

	for (list<Snowflake *>::iterator i = snowflakes.begin(); i != snowflakes.end(); i++)
		(*i)->snowflake->draw(camera, lights, fog);
}

glm::mat4 SnowGenerator::getPlacementMatrix(Snowflake * snowflake, glm::vec3 position, glm::vec3 direction)
{
	float angle = glm::degrees(glm::acos((glm::dot(snowflake->previousDirection, direction)) / (glm::dot(glm::length(snowflake->previousDirection), glm::length(direction)))));
	glm::vec3 axis = glm::cross(snowflake->previousDirection, direction);
	//glm::mat4 rotation = glm::rotate(glm::mat4(1), angle, axis);

	snowflake->previousDirection = direction;

	return glm::rotate(glm::translate(glm::scale(glm::mat4(1), glm::vec3(5, 5, 5)), position), angle, axis);
}
