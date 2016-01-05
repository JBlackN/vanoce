//----------------------------------------------------------------------------------------
/**
* \file       SnowGenerator.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains SnowGenerator class.
*
*  File contains SnowGenerator class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <cstdlib>
#include <string>
#include <list>
#include <map>
using namespace std;

#include "pgr.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "Fog.h"
#include "Spline.h"
#include "Collision.h"

/**
 * Class provides random snowflake generation.
 */
class SnowGenerator
{
public:
	/**
	 * Structure describes one snowflake.
	 */
	struct Snowflake {
		Object * snowflake;
		Spline * path;
		int frame; ///< Current animation frame.
		glm::vec3 previousDirection;

		Snowflake(Object * snowflake, Spline * path, int frame)
		{
			this->snowflake = snowflake;
			this->path = path;
			this->frame = frame;
			this->previousDirection = glm::vec3(0);
		}

		~Snowflake()
		{
			delete path;
			delete snowflake;
		}
	};
private:
	Config * config; ///< Config provider.
	bool enabled;

	Model * snowflake;
	list<Snowflake *> snowflakes;

	float fallHeight; ///< Height from which snowflakes start to fall.
	int snowflakeCount; ///< Number of snowflakes in the scene.
	int frameCount; ///< Total number of animation frames.

	Collision * collision; ///< Collision checker.
public:
	SnowGenerator(Config * config, Model * snowflake, float fallHeight, int snowflakeCount, int secondsToFall, int fps,
		Object * collideWith = NULL, bool enabled = true);
	~SnowGenerator();

	void enable();
	void disable();
	bool isEnabled();

	void generate();

	/**
	 * Updates position and direction of every snowflake based on current animation frame.
	 */
	void update();

	/**
	 * Removes snowflake from the scene after it falls to the ground or if it collides with home object.
	 * @param i Iterator pointing to the position of snowflake being removed.
	 */
	void remove(list<Snowflake *>::iterator i);

	void draw(Camera * camera, map<string, Light *> lights, Fog * fog);
private:
	/**
	 * Gets snowflake's updated position and direction in the form of placement matrix.
	 * @param snowflake Snowflake to get matrix for.
	 * @param position Snowflake's new position.
	 * @param direction Snowflake's new direction.
	 * @return 4x4 placement matrix.
	 */
	glm::mat4 getPlacementMatrix(Snowflake * snowflake, glm::vec3 position, glm::vec3 direction);
};

