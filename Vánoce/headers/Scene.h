//----------------------------------------------------------------------------------------
/**
* \file       Scene.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Scene class.
*
*  File contains Scene class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <list>
#include <map>
using namespace std;

#include "pgr.h"
#include "Config.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Model.h"
#include "Object.h"
#include "Light.h"
#include "Fog.h"
#include "Camera.h"
#include "TreeGenerator.h"
#include "SnowGenerator.h"
#include "Inventory.h"
#include "Hud.h"
#include "Overlay.h"
#include "Frame.h"

#include "data\models\skybox.h"
#include "data\models\terrain.h"
#include "data\models\home.h"
#include "data\models\table.h"
#include "data\models\chair.h"
#include "data\models\carton.h"
#include "data\models\ornament.h"
#include "data\models\stand.h"
#include "data\models\tree.h"
#include "data\models\gift.h"
#include "data\models\snowflake.h"
#include "data\models\picture.h"

/**
 * Class holds and manages the entire scene.
 */
class Scene
{
public:
	Config * config;

	map<string, Shader *> shaders;
	map<string, Material *> materials;
	map<string, Texture *> textures;
	map<string, Model *> models;
	map<string, Object *> objects;
	map<string, Camera *> cameras;
	map<string, Light *> lights;
	Fog * fog;

	Camera * activeCamera;

	TreeGenerator * treeGenerator;
	SnowGenerator * snowGenerator;
	Inventory * inventory;

	Hud * hud;
	Overlay * overlay; ///< Animated overlay.
	Frame * frame; ///< Animated picture frame.
public:
	Scene(glm::vec2 window_dimensions, void (*timerFunc)(int));
	~Scene();

	void reload(glm::vec2 window_dimensions);
	void reshape(glm::vec2 window_dimensions);
	void draw(glm::vec2 window_dimensions);
private:
	void loadShaders();
	void loadMaterials();
	void loadTextures();
	void loadModels();
	void createObjects();
	void createCameras(glm::vec2 window_dimensions);
	void createLights();
	void createFog();
	void initInventory();
	void initHud(glm::vec2 window_dimensions);
	void initOverlay(glm::vec2 window_dimensions);
	void initGenerators();

	/**
	 * Gets placement matrix consisting of scale and translate.
	 * @param position Object position in the world.
	 * @return 4x4 placement matrix.
	 */
	glm::mat4 getPlacementMatrix(glm::vec3 position);
};

