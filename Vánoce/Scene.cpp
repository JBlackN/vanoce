#include "headers\Scene.h"

Scene::Scene(glm::vec2 window_dimensions, void (*timerFunc)(int))
{
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

	glutTimerFunc(1000.0 / 25.0, timerFunc, 0);
}

Scene::~Scene()
{
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

	hud->draw(inventory, 0, window_dimensions.x, window_dimensions.y, 0, -1.0f, 100.0f);
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
	textures["skybox"] = new Texture("textures/skybox.jpg", true);
	textures["snow"] = new Texture("textures/snow.png");
	textures["home"] = new Texture("textures/home.png");
	textures["wood"] = new Texture("textures/wood.jpg");
	textures["fabric"] = new Texture("textures/fabric.png");
	textures["cardboard"] = new Texture("textures/cardboard.jpg");
	textures["glass"] = new Texture("textures/glass.png");
	textures["metal"] = new Texture("textures/metal.png");
	textures["tree"] = new Texture("textures/tree.png");
	textures["gift"] = new Texture("textures/gift.png");
	textures["overlay_text"] = new Texture("textures/overlay_text.png");
	textures["pictures"] = new Texture("textures/pictures.png");
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
	objects["skybox"] = new Object(models["skybox"], glm::scale(glm::rotate(glm::translate(glm::mat4(), glm::vec3(0, 25, 0)),
		90.0f, glm::vec3(-1, 0, 0)), glm::vec3(100, 100, 100)));

	objects["terrain"] = new Object(models["terrain"], glm::scale(glm::rotate(glm::mat4(), 90.0f, glm::vec3(-1, 0, 0)),
		glm::vec3(100, 100, 1)));

	objects["home"] = new Object(models["home"], glm::scale(glm::mat4(), glm::vec3(5, 5, 5)));

	objects["table"] = new Object(models["table"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(-1.5f, 0.5f, 0)));

	objects["chair"] = new Object(models["chair"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(0, 0.5f, 0)));

	objects["carton"] = new Object(models["carton"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 3, 5)),
		glm::vec3(-0.9f, 1.75f, -0.6f)));

	objects["ornament_red"] = new Object(models["ornament_red"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(-0.9f, 1.0f, -0.55f)));
	
	objects["ornament_yellow"] = new Object(models["ornament_yellow"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(-1.0f, 1.0f, -0.55f)));
	
	objects["ornament_blue"] = new Object(models["ornament_blue"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(-0.97f, 1.0f, -0.67f)));

	objects["stand"] = new Object(models["stand"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(1.5f, 0.2f, -1.5f)));
	
	objects["gift"] = new Object(models["gift"], glm::rotate(glm::translate(glm::scale(glm::mat4(),
		glm::vec3(5, 5, 5)), glm::vec3(1.3f, 0.2f, -0.8f)), 45.0f, glm::vec3(0, 1, 0)));

	objects["christmasTree"] = new Object(models["tree"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(1.5f, 1.4f, -1.5f)));

	frame = new Frame(models["frame"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(2.5f, 1.7f, 0.8f)));
}

void Scene::createCameras(glm::vec2 window_dimensions)
{
	float window_aspectRatio = window_dimensions.x / window_dimensions.y;

	cameras["fps"] = new Camera(50.0f, window_aspectRatio, 0.1f, 1000.0f,
		glm::vec3(2, 7, -50), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0), true, true);
	cameras["inside"] = new Camera(100.0f, window_aspectRatio, 0.1f, 1000.0f,
		glm::vec3(-5, 10, 5), glm::vec3(1.5f, 4, -1.5f), glm::vec3(0, 1, 0), false, true);
	cameras["outside"] = new Camera(35.0f, window_aspectRatio, 0.1f, 1000.0f,
		glm::vec3(100, 100, -100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

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
	fog = new Fog(0.015f, glm::vec4(0, 0, 0, 1));
}

void Scene::initInventory()
{
	inventory = new Inventory(models["ornament_red"], models["ornament_yellow"], models["ornament_blue"]);
}

void Scene::initHud(glm::vec2 window_dimensions)
{
	map<string, Material *> hudMaterials;
	hudMaterials["ornament_red_hud"] = new Material(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0), 0, glm::vec3(0));
	hudMaterials["ornament_yellow_hud"] = new Material(glm::vec3(0), glm::vec3(1, 0.7f, 0), glm::vec3(0), 0, glm::vec3(0));
	hudMaterials["ornament_blue_hud"] = new Material(glm::vec3(0), glm::vec3(0.2f, 0.2f, 1), glm::vec3(0), 0, glm::vec3(0));

	map<string, Texture *> hudTextures;
	hudTextures["ornament_red_hud"] = new Texture("textures/ornament_red.png");
	hudTextures["ornament_yellow_hud"] = new Texture("textures/ornament_yellow.png");
	hudTextures["ornament_blue_hud"] = new Texture("textures/ornament_blue.png");
	hudTextures["ornament_1_hud"] = new Texture("textures/ornament_1.png");
	hudTextures["ornament_2_hud"] = new Texture("textures/ornament_2.png");
	hudTextures["ornament_3_hud"] = new Texture("textures/ornament_3.png");
	hudTextures["ornament_4_hud"] = new Texture("textures/ornament_4.png");
	hudTextures["ornament_5_hud"] = new Texture("textures/ornament_5.png");
	hudTextures["ornament_6_hud"] = new Texture("textures/ornament_6.png");
	hudTextures["ornament_7_hud"] = new Texture("textures/ornament_7.png");
	hudTextures["ornament_8_hud"] = new Texture("textures/ornament_8.png");
	hudTextures["ornament_9_hud"] = new Texture("textures/ornament_9.png");

	hud = new Hud(window_dimensions.x, window_dimensions.y, shaders["hud"], hudMaterials, hudTextures);
}

void Scene::initOverlay(glm::vec2 window_dimensions)
{
	map<string, Texture *> overlayTextures;
	overlayTextures["bg"] = textures["glass"];
	overlayTextures["text"] = textures["overlay_text"];

	overlay = new Overlay(window_dimensions.x, window_dimensions.y, shaders["overlay"], materials["ornament_blue"],
		overlayTextures, false, 25, 5, 25);
}

void Scene::initGenerators()
{
	snowGenerator = new SnowGenerator(models["snowflake"], 10.0f, 30, 20, 25, objects["home"]);
	treeGenerator = new TreeGenerator(models["tree"], 19);
	treeGenerator->generateTrees(50);
}
