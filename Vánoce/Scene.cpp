#include "headers\Scene.h"

Scene::Scene(glm::vec2 window_dimensions, void (*timerFunc)(int))
{
	config = new Config();

	loadShaders();
	loadMaterials();
	loadTextures();
	initHud(window_dimensions);
	loadModels();
	createObjects();
	createCameras(window_dimensions);
	createLights();
	createFog();
	initInventory();
	initOverlay(window_dimensions);
	initGenerators();

	glutTimerFunc(1000.0 / config->fOpt("fps"), timerFunc, 0);
}

Scene::~Scene()
{
	delete snowGenerator;
	delete treeGenerator;
	delete overlay;
	delete inventory;
	delete fog;

	lights.clear();
	cameras.clear();
	objects.clear();

	delete frame;
	delete hud;
	
	models.clear();

	textures.clear();
	materials.clear();
	shaders.clear();

	delete config;
}

void Scene::reload(glm::vec2 window_dimensions)
{
	delete treeGenerator;
	delete snowGenerator;
	delete overlay;
	delete inventory;
	delete fog;
	cameras.clear();
	objects.clear();
	delete config;

	config = new Config();
	hud->reload(window_dimensions.x, window_dimensions.y,
		config->fOpt("hud_element_size_px"), config->fOpt("hud_element_border_percent"));
	createObjects();
	createCameras(window_dimensions);
	createFog();
	initInventory();
	initOverlay(window_dimensions);
	initGenerators();
}

void Scene::reshape(glm::vec2 window_dimensions)
{
	delete overlay;

	hud->reload(window_dimensions.x, window_dimensions.y,
		config->fOpt("hud_element_size_px"), config->fOpt("hud_element_border_percent"));

	for (map<string, Camera *>::iterator i = cameras.begin(); i != cameras.end(); i++)
		(*i).second->aspectRatio = (float)window_dimensions.x / (float)window_dimensions.y;

	initOverlay(window_dimensions);
}

void Scene::draw(glm::vec2 window_dimensions)
{
	glDepthMask(0);
	objects["skybox"]->draw(activeCamera);
	glDepthMask(1);

	objects["terrain"]->draw(activeCamera, lights, fog);
	objects["home"]->draw(activeCamera, lights, fog);
	objects["stand"]->draw(activeCamera, lights, fog);
	objects["table"]->draw(activeCamera, lights, fog);
	objects["chair"]->draw(activeCamera, lights, fog);
	objects["carton"]->draw(activeCamera, lights, fog);
	frame->draw(activeCamera, lights, fog);

	snowGenerator->draw(activeCamera, lights, fog);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glStencilFunc(GL_ALWAYS, 1, -1);
	objects["ornament_red"]->draw(activeCamera, lights, fog);
	glStencilFunc(GL_ALWAYS, 2, -1);
	objects["ornament_yellow"]->draw(activeCamera, lights, fog);
	glStencilFunc(GL_ALWAYS, 3, -1);
	objects["ornament_blue"]->draw(activeCamera, lights, fog);

	glStencilFunc(GL_ALWAYS, 4, -1);
	objects["gift"]->draw(activeCamera, lights, fog);

	glStencilFunc(GL_ALWAYS, 5, -1);
	objects["christmasTree"]->draw(activeCamera, lights, fog);
	treeGenerator->drawTrees(activeCamera, lights, fog, 6);

	glDisable(GL_STENCIL_TEST);

	inventory->drawOrnaments(activeCamera, lights, fog);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	hud->draw(inventory, 0, window_dimensions.x, window_dimensions.y, 0);
	overlay->draw(0, window_dimensions.x, window_dimensions.y, 0);

	glDisable(GL_BLEND);
}

void Scene::loadShaders()
{
	shaders["generic"] = new Shader("shaders/generic.vert", "shaders/generic.frag");
	shaders["skybox"] = new Shader("shaders/generic.vert", "shaders/skybox.frag");
	shaders["hud"] = new Shader("shaders/generic.vert", "shaders/hud.frag");
	shaders["overlay"] = new Shader("shaders/animatedTexture.vert", "shaders/overlay.frag");
	shaders["frame"] = new Shader("shaders/animatedTexture.vert", "shaders/generic.frag");
}

void Scene::loadMaterials()
{
	materials["skybox"] = new Material(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 0);
	materials["snow"] = new Material(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 10);
	materials["snowflake"] = new Material(glm::vec3(0, 0, 0.2f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 10, glm::vec3(0.5f, 0.5f, 0.5f));
	materials["home"] = new Material(glm::vec3(0, 0, 0.4f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 1);
	materials["wood"] = new Material(glm::vec3(0, 0, 0.2f), glm::vec3(0.6f, 0.4f, 0.4f), glm::vec3(0.8f, 0.6f, 0.4f), 1);
	materials["fabric"] = new Material(glm::vec3(0, 0.2f, 0.4f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 1);
	materials["cardboard"] = new Material(glm::vec3(0.2f, 0.2f, 0), glm::vec3(0.6f, 0.4f, 0.2f), glm::vec3(0.8f, 0.6f, 0.4f), 5);
	materials["ornament_red"] = new Material(glm::vec3(0.2f, 0, 0.2f), glm::vec3(0.7f, 0, 0.2f), glm::vec3(0.6f, 0.2f, 0.1f), 51.2f);
	materials["ornament_yellow"] = new Material(glm::vec3(0.6f, 0.5f, 0), glm::vec3(1, 0.9f, 0), glm::vec3(1, 1, 0.4f), 51.2f);
	materials["ornament_blue"] = new Material(glm::vec3(0.2f, 0.2f, 0.4f), glm::vec3(0.2f, 0.2f, 1), glm::vec3(0.2f, 0.6f, 1), 51.2f);
	materials["metal"] = new Material(glm::vec3(0, 0, 0.2f), glm::vec3(0, 0.1f, 0.3f), glm::vec3(1, 1, 1), 15);
	materials["tree"] = new Material(glm::vec3(0, 0.2f, 0), glm::vec3(0.2f, 0.4f, 0.2f), glm::vec3(0.2f, 0.4f, 0.4f), 1);
	materials["gift"] = new Material(glm::vec3(0.3f, 0.1f, 0), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 2);
}

void Scene::loadTextures()
{
	textures["skybox"] = new Texture("data/textures/skybox.jpg", true);
	textures["snow"] = new Texture("data/textures/snow.png");
	textures["home"] = new Texture("data/textures/home.png");
	textures["wood"] = new Texture("data/textures/wood.jpg");
	textures["fabric"] = new Texture("data/textures/fabric.png");
	textures["cardboard"] = new Texture("data/textures/cardboard.jpg");
	textures["glass"] = new Texture("data/textures/glass.png");
	textures["metal"] = new Texture("data/textures/metal.png");
	textures["tree"] = new Texture("data/textures/tree.png");
	textures["gift"] = new Texture("data/textures/gift.png");
	textures["overlay_text"] = new Texture("data/textures/overlay_text.png");
	textures["pictures"] = new Texture("data/textures/pictures.png");
}

void Scene::loadModels()
{
	models["skybox"] = new Model(shaders["skybox"], materials["skybox"], textures["skybox"],
		skyboxNAttribsPerVertex, skyboxNVertices, skyboxNTriangles, skyboxVertices, skyboxTriangles);

	models["terrain"] = new Model(shaders["generic"], materials["snow"], textures["snow"],
		terrainNAttribsPerVertex, terrainNVertices, terrainNTriangles, terrainVertices, terrainTriangles);

	models["home"] = new Model(shaders["generic"], materials["home"], textures["home"],
		homeNAttribsPerVertex, homeNVertices, homeNTriangles, homeVertices, homeTriangles);

	models["table"] = new Model(shaders["generic"], materials["wood"], textures["wood"],
		tableNAttribsPerVertex, tableNVertices, tableNTriangles, tableVertices, tableTriangles);

	models["chair"] = new Model(shaders["generic"], materials["fabric"], textures["fabric"],
		chairNAttribsPerVertex, chairNVertices, chairNTriangles, chairVertices, chairTriangles);

	models["carton"] = new Model(shaders["generic"], materials["cardboard"], textures["cardboard"],
		cartonNAttribsPerVertex, cartonNVertices, cartonNTriangles, cartonVertices, cartonTriangles);

	models["ornament_red"] = new Model(shaders["generic"], materials["ornament_red"], textures["glass"],
		ornamentNAttribsPerVertex, ornamentNVertices, ornamentNTriangles, ornamentVertices, ornamentTriangles);

	models["ornament_yellow"] = new Model(shaders["generic"], materials["ornament_yellow"], textures["glass"],
		ornamentNAttribsPerVertex, ornamentNVertices, ornamentNTriangles, ornamentVertices, ornamentTriangles);

	models["ornament_blue"] = new Model(shaders["generic"], materials["ornament_blue"], textures["glass"],
		ornamentNAttribsPerVertex, ornamentNVertices, ornamentNTriangles, ornamentVertices, ornamentTriangles);

	models["stand"] = new Model(shaders["generic"], materials["metal"], textures["metal"],
		standNAttribsPerVertex, standNVertices, standNTriangles, standVertices, standTriangles);

	models["tree"] = new Model(shaders["generic"], materials["tree"], textures["tree"],
		treeNAttribsPerVertex, treeNVertices, treeNTriangles, treeVertices, treeTriangles);

	models["gift"] = new Model(shaders["generic"], materials["gift"], textures["gift"],
		giftNAttribsPerVertex, giftNVertices, giftNTriangles, giftVertices, giftTriangles);

	models["snowflake"] = new Model(shaders["generic"], materials["snowflake"], textures["snow"],
		snowflakeNAttribsPerVertex, snowflakeNVertices, snowflakeNTriangles, snowflakeVertices, snowflakeTriangles);

	models["frame"] = new Model(shaders["frame"], materials["wood"], textures["pictures"],
		pictureNAttribsPerVertex, pictureNVertices, pictureNTriangles, pictureVertices, pictureTriangles);
}

void Scene::createObjects()
{
	objects["skybox"] = new Object(models["skybox"], glm::scale(glm::rotate(glm::translate(glm::mat4(1),
		config->vOpt("skybox_pos")), 90.0f, glm::vec3(-1, 0, 0)), config->fOpt("scale") * glm::vec3(100, 100, 100)));

	objects["terrain"] = new Object(models["terrain"], glm::scale(glm::rotate(glm::mat4(1), 90.0f, glm::vec3(-1, 0, 0)),
		config->fOpt("scale") * glm::vec3(100, 100, 1)));

	objects["carton"] = new Object(models["carton"], glm::translate(glm::scale(glm::mat4(1),
		config->fOpt("scale") * glm::vec3(5, 3, 5)), config->vOpt("carton_pos")));

	objects["gift"] = new Object(models["gift"], glm::rotate(glm::translate(glm::scale(glm::mat4(1),
		config->fOpt("scale") * glm::vec3(5, 5, 5)), config->vOpt("gift_pos")),
		config->fOpt("gift_rotation"), glm::vec3(0, 1, 0)));

	objects["home"] = new Object(models["home"], getPlacementMatrix(config->vOpt("home_pos")));
	objects["table"] = new Object(models["table"], getPlacementMatrix(config->vOpt("table_pos")));
	objects["chair"] = new Object(models["chair"], getPlacementMatrix(config->vOpt("chair_pos")));
	objects["ornament_red"] = new Object(models["ornament_red"], getPlacementMatrix(config->vOpt("r_ornament_pos")));
	objects["ornament_yellow"] = new Object(models["ornament_yellow"], getPlacementMatrix(config->vOpt("y_ornament_pos")));
	objects["ornament_blue"] = new Object(models["ornament_blue"], getPlacementMatrix(config->vOpt("b_ornament_pos")));
	objects["stand"] = new Object(models["stand"], getPlacementMatrix(config->vOpt("stand_pos")));
	objects["christmasTree"] = new Object(models["tree"], getPlacementMatrix(config->vOpt("christmas_tree_pos")));

	frame = new Frame(models["frame"], getPlacementMatrix(config->vOpt("frame_pos")));
}

void Scene::createCameras(glm::vec2 window_dimensions)
{
	float window_aspectRatio = window_dimensions.x / window_dimensions.y;

	cameras["fps"] = new Camera(config, config->fOpt("fps_fov"), window_aspectRatio, 0.1f, 1000.0f,
		config->vOpt("fps_pos"), config->vOpt("fps_center"), glm::vec3(0, 1, 0), true, true);
	cameras["inside"] = new Camera(config, config->fOpt("in_fov"), window_aspectRatio, 0.1f, 1000.0f,
		config->vOpt("in_pos"), config->vOpt("in_center"), glm::vec3(0, 1, 0), false, true);
	cameras["outside"] = new Camera(config, config->fOpt("out_fov"), window_aspectRatio, 0.1f, 1000.0f,
		config->vOpt("out_pos"), config->vOpt("out_center"), glm::vec3(0, 1, 0));
	cameras["animated"] = new Camera(config, config->fOpt("cam_anim_fov"), window_aspectRatio, 0.1f, 1000.0f,
		config->vOpt("out_pos"), config->vOpt("out_center"), glm::vec3(0, 1, 0), false, false, false, true);
	cameras["free"] = new Camera(config, config->fOpt("cam_free_fov"), window_aspectRatio, 0.1f, 1000.0f,
		config->vOpt("cam_free_pos"), config->vOpt("cam_free_center"), glm::vec3(0, 1, 0), true, true, true, false);

	activeCamera = cameras["fps"];
}

void Scene::createLights()
{
	lights["moon"] = new Light(Light::LightType::directional, glm::vec3(100, 50, 100),
		glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.15f), glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0), 1, 360, 0, 0, 0);
	lights["lamp"] = new Light(Light::LightType::point, glm::vec3(0, 8, 0),
		glm::vec3(0, 0, 0.2f), glm::vec3(1, 0.6f, 0.2f), glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0), 1, 180, 1, 0, 0);
	lights["flashlight"] = new Light(Light::LightType::spot, cameras["fps"]->position,
		glm::vec3(0, 0, 0.2f), glm::vec3(0.8f, 1, 1), glm::vec3(0.1f, 0.1f, 0.1f),
		cameras["fps"]->center - cameras["fps"]->position, 0.9f, 0.5f, 0, 0, 2.5f, false);
}

void Scene::createFog()
{
	fog = new Fog(config->fOpt("fog_density"), glm::vec4(config->vOpt("fog_color"), 1));
}

void Scene::initInventory()
{
	inventory = new Inventory(config, models["ornament_red"], models["ornament_yellow"], models["ornament_blue"]);
}

void Scene::initHud(glm::vec2 window_dimensions)
{
	map<string, Material *> hudMaterials;
	hudMaterials["ornament_red_hud"] = new Material(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0), 0, glm::vec3(0));
	hudMaterials["ornament_yellow_hud"] = new Material(glm::vec3(0), glm::vec3(1, 0.7f, 0), glm::vec3(0), 0, glm::vec3(0));
	hudMaterials["ornament_blue_hud"] = new Material(glm::vec3(0), glm::vec3(0.2f, 0.2f, 1), glm::vec3(0), 0, glm::vec3(0));

	map<string, Texture *> hudTextures;
	hudTextures["ornament_red_hud"] = new Texture("data/textures/ornament_red.png");
	hudTextures["ornament_yellow_hud"] = new Texture("data/textures/ornament_yellow.png");
	hudTextures["ornament_blue_hud"] = new Texture("data/textures/ornament_blue.png");
	hudTextures["ornament_1_hud"] = new Texture("data/textures/ornament_1.png");
	hudTextures["ornament_2_hud"] = new Texture("data/textures/ornament_2.png");
	hudTextures["ornament_3_hud"] = new Texture("data/textures/ornament_3.png");
	hudTextures["ornament_4_hud"] = new Texture("data/textures/ornament_4.png");
	hudTextures["ornament_5_hud"] = new Texture("data/textures/ornament_5.png");
	hudTextures["ornament_6_hud"] = new Texture("data/textures/ornament_6.png");
	hudTextures["ornament_7_hud"] = new Texture("data/textures/ornament_7.png");
	hudTextures["ornament_8_hud"] = new Texture("data/textures/ornament_8.png");
	hudTextures["ornament_9_hud"] = new Texture("data/textures/ornament_9.png");

	hud = new Hud(window_dimensions.x, window_dimensions.y, shaders["hud"], hudMaterials, hudTextures,
		config->fOpt("hud_element_size_px"), config->fOpt("hud_element_border_percent"));
}

void Scene::initOverlay(glm::vec2 window_dimensions)
{
	map<string, Texture *> overlayTextures;
	overlayTextures["bg"] = textures["glass"];
	overlayTextures["text"] = textures["overlay_text"];

	overlay = new Overlay(window_dimensions.x, window_dimensions.y, shaders["overlay"], materials["ornament_blue"],
		overlayTextures, false, config->fOpt("fps"), config->fOpt("overlay_secs"), config->fOpt("overlay_magnification"));
}

void Scene::initGenerators()
{
	snowGenerator = new SnowGenerator(config, models["snowflake"], config->fOpt("snowflake_fall_height"),
		config->fOpt("snowflake_count"), config->fOpt("snowflake_secs"), config->fOpt("fps"), objects["home"]);
	treeGenerator = new TreeGenerator(config, models["tree"], 19);
	treeGenerator->generateTrees(config->fOpt("tree_count"));
}

glm::mat4 Scene::getPlacementMatrix(glm::vec3 position)
{
	return glm::translate(glm::scale(glm::mat4(1), config->fOpt("scale") * glm::vec3(5, 5, 5)), position);
}