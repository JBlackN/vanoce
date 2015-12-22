#pragma once

#include <vector>
#include <list>
using namespace std;

#include "pgr.h"
#include "Config.h"
#include "Object.h"

/**
 * Class describes point-cube and sphere-sphere collisions.
 */
class Collision
{
private:
	Config * config; ///< Config provider.

	glm::vec3 homeBoundariesMin; ///< Lower boundaries of home object.
	glm::vec3 homeBoundariesMax; ///< Upper boundaries of home object.

	float ornamentDiameter;
	list<glm::vec3> ornamentPositions; ///< Positions of ornaments placed within the scene.
public:
	/**
	 * Point-cube collision constructor.
	 * @param config Config provider.
	 * @param object Cube to check for collisions with.
	 */
	Collision(Config * config, Object * object);

	/**
	* Sphere-sphere collision constructor.
	* @param config Config provider.
	* @param nAttrPerVertex Values per vertex.
	* @param nVert Number of vertices.
	* @param vertices Array of vertices.
	*/
	Collision(Config * config, int nAttrPerVertex, int nVert, const float * vertices);
	~Collision();

	/**
	 * Check if point lies within the cube.
	 * @param world_position Position of the point.
	 * @return Whether point lies within the cube.
	 */
	bool check(glm::vec3 world_position);

	/**
	 * Check if new sphere collides with any of the already placed spheres.
	 * @param position Position of a new sphere.
	 * @return Whether new sphere collides with any of the other spheres within the scene.
	 */
	bool sphereCheck(glm::vec3 position);

	/**
	 * Records sphere's position.
	 * @param position Position to record.
	 */
	void addOrnamentPosition(glm::vec3 position);
};

