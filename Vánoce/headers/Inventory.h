#pragma once

#include <string>
#include <map>
#include <list>
using namespace std;

#include "pgr.h"
#include "Config.h"
#include "Model.h"
#include "Object.h"
#include "Camera.h"
#include "Material.h"
#include "Light.h"
#include "Fog.h"
#include "Collision.h"

/**
 * Class describes inventory which can contain red, yellow and blue ornaments.
 */
class Inventory
{
public:
	enum OrnamentType { red, yellow, blue };
private:
	Config * config; ///< Config provider.

	Model * redOrnament;
	Model * yellowOrnament;
	Model * blueOrnament;

	unsigned int nRedOrnaments; ///< Count of red ornaments placed in inventory.
	unsigned int nYellowOrnaments; ///< Count of yellow ornaments placed in inventory.
	unsigned int nBlueOrnaments; ///< Count of blue ornaments placed in inventory.

	map<OrnamentType, list<Object *>> placedOrnaments; ///< Ornaments by color placed within the scene.
	Collision * collision; ///< Ornament collision checker.
public:
	Inventory(Config * config, Model * redOrnament, Model * yellowOrnament, Model * blueOrnament);
	~Inventory();

	/**
	 * Adds ornament to inventory.
	 * @param type Ornament color (red, yellow, blue).
	 * @param count How many ornaments to add.
	 */
	void insertOrnament(OrnamentType type, unsigned int count = 1);

	/**
	 * Removes ornament from inventory.
	 * @param type Ornament color (red, yellow, blue).
	 */
	void removeOrnament(OrnamentType type);

	/**
	 * Gets number of ornaments (of specified color) within inventory.
	 * @param type Ornament color (red, yellow, blue).
	 * @return Number of ornaments (of specified color) within inventory.
	 */
	unsigned int ornamentCount(OrnamentType type);

	/**
	 * Places ornament from inventory to the scene (on a specified tree).
	 * @param type Ornament color (red, yellow, blue).
	 * @param tree Tree to be decorated.
	 * @param window_dimensions Current window dimensions.
	 * @param windowX Mouse click x-coordinate.
	 * @param windowY Mouse click y-coordinate.
	 * @param camera Active camera.
	 */
	void placeOrnament(OrnamentType type, Object * tree, glm::vec2 window_dimensions, int windowX, int windowY, Camera * camera);
	
	void drawOrnaments(Camera * camera, map<string, Light *> lights, Fog * fog);
private:
	/**
	 * Determines position within the scene where to place an ornament.
	 * @param tree Tree to be decorated.
	 * @param window_dimensions Current window dimensions.
	 * @param windowX Mouse click x-coordinate.
	 * @param windowY Mouse click y-coordinate.
	 * @param camera Active camera.
	 * @return Position.
	 */
	glm::vec3 findPosition(Object * tree, glm::vec2 window_dimensions, int windowX, int windowY, Camera * camera);
};

