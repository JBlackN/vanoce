#include <iostream>
#include <ctime>
#include <string>
#include <map>
using namespace std;

#include "pgr.h"
#include "headers\Shader.h"
#include "headers\Material.h"
#include "headers\Texture.h"
#include "headers\Model.h"
#include "headers\Object.h"
#include "headers\TreeGenerator.h"
#include "headers\Camera.h"
#include "headers\Light.h"
#include "headers\Fog.h"
#include "headers\Inventory.h"
#include "headers\Hud.h"
#include "headers\Spline.h"
#include "headers\SnowGenerator.h"

#include "models\skybox.h"
#include "models\terrain.h"
#include "models\home.h"
#include "models\table.h"
#include "models\chair.h"
#include "models\carton.h"
#include "models\ornament.h"
#include "models\stand.h"
#include "models\tree.h"
#include "models\gift.h"
#include "models\snowflake.h"

const glm::vec2 window_dimensions = glm::vec2(800.0f, 600.0f);
glm::vec2 cursor_position = glm::vec2(0.0f, 0.0f);

map<string, Shader *> shaders;
map<string, Material *> materials;
map<string, Texture *> textures;
map<string, Model *> models;
map<string, Object *> objects;
map<string, Light *> lights;
Fog * fog;
map<string, Camera *> cameras;
Camera * activeCamera;

TreeGenerator * treeGenerator;
SnowGenerator * snowGenerator;

Inventory * inventory;
Hud * hud;

/*int frame = 0;
int frameCount = 25 * 20;
glm::vec3 snowflakeTest = glm::vec3(0, 5, -5);
Spline * spline = new Spline(snowflakeTest, 5, 30);*/

void init(void);

void displayFunc(void);
void reshapeFunc(int, int);
void keyboardFunc(unsigned char, int, int);
void keyboardSpecialFunc(int, int, int);
void mouseFunc(int, int, int, int);
void motionFunc(int, int);
void idleFunc(void);
void timerFunc(int);

int main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);

	glutInitWindowSize(window_dimensions.x, window_dimensions.y);
	glutCreateWindow("Window");

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(keyboardSpecialFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc); // TODO
	//glutIdleFunc(idleFunc);
	//glutTimerFunc(40, timerFunc, frame);

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	init();

	glutMainLoop();

	return 0;
}

void init()
{
	// Shaders

	shaders["generic"] = new Shader("shaders/generic.vert", "shaders/generic.frag");
	shaders["skybox"] = new Shader("shaders/generic.vert", "shaders/skybox.frag");
	shaders["hud"] = new Shader("shaders/generic.vert", "shaders/hud.frag");

	// Materials

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

	map<string, Material *> hudMaterials;
	hudMaterials["ornament_red_hud"] = new Material(glm::vec3(0), glm::vec3(1, 0, 0), glm::vec3(0), 0, glm::vec3(0));
	hudMaterials["ornament_yellow_hud"] = new Material(glm::vec3(0), glm::vec3(1, 0.7f, 0), glm::vec3(0), 0, glm::vec3(0));
	hudMaterials["ornament_blue_hud"] = new Material(glm::vec3(0), glm::vec3(0.2f, 0.2f, 1), glm::vec3(0), 0, glm::vec3(0));

	//Textures

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

	// Models

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

	// Objects

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
	treeGenerator = new TreeGenerator(models["tree"]);
	treeGenerator->generateTrees(50, 19);

	snowGenerator = new SnowGenerator(models["snowflake"], 10.0f, 30, 20, 25);
	glutTimerFunc(1000.0 / 25.0, timerFunc, 0);

	// Cameras

	float window_aspectRatio = window_dimensions.x / window_dimensions.y;

	cameras["fps"] = new Camera(50.0f, window_aspectRatio, 0.1f, 1000.0f,
		glm::vec3(2, 7, -50), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0), true, true);
	cameras["inside"] = new Camera(100.0f, window_aspectRatio, 0.1f, 1000.0f,
		glm::vec3(-5, 10, 5), glm::vec3(1.5f, 4, -1.5f), glm::vec3(0, 1, 0), false, true);
	cameras["outside"] = new Camera(35.0f, window_aspectRatio, 0.1f, 1000.0f,
		glm::vec3(100, 100, -100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	activeCamera = cameras["fps"];

	// Lights

	lights["moon"] = new Light(Light::LightType::directional, glm::vec3(100, 50, 100),
		glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.1f, 0.1f, 0.15f), glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0), 1, 360, 0, 0, 0);
	lights["lamp"] = new Light(Light::LightType::point, glm::vec3(0, 8, 0),
		glm::vec3(0, 0, 0.2f), glm::vec3(1, 0.6f, 0.2f), glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0), 1, 180, 1, 0, 0);
	lights["flashlight"] = new Light(Light::LightType::spot, cameras["fps"]->position,
		glm::vec3(0, 0, 0.2f), glm::vec3(0.8f, 1, 1), glm::vec3(0.1f, 0.1f, 0.1f),
		cameras["fps"]->center - cameras["fps"]->position, 0.9f, 0.5f, 0, 0, 2.5f, false);

	// Fog

	fog = new Fog(0.015f, glm::vec4(0, 0, 0, 1));

	// Inventory

	inventory = new Inventory(models["ornament_red"], models["ornament_yellow"], models["ornament_blue"]);

	// HUD

	hud = new Hud(window_dimensions.x, window_dimensions.y, shaders["hud"], hudMaterials, hudTextures);
}

void displayFunc()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glDepthMask(0);
	objects["skybox"]->draw(activeCamera);
	glDepthMask(1);

	objects["terrain"]->draw(activeCamera, lights, fog);
	objects["home"]->draw(activeCamera, lights, fog);
	objects["stand"]->draw(activeCamera, lights, fog);
	objects["table"]->draw(activeCamera, lights, fog);
	objects["chair"]->draw(activeCamera, lights, fog);
	objects["carton"]->draw(activeCamera, lights, fog);
	objects["gift"]->draw(activeCamera, lights, fog);

	/*objects["snowflake"]->adjustmentMatrix = glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)), snowflakeTest);
	objects["snowflake"]->draw(activeCamera, lights, fog);*/
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
	objects["christmasTree"]->draw(activeCamera, lights, fog);
	treeGenerator->drawTrees(activeCamera, lights, fog, true);

	glDisable(GL_STENCIL_TEST);

	inventory->drawOrnaments(activeCamera, lights, fog);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	hud->draw(inventory, 0, window_dimensions.x, window_dimensions.y, 0, -1.0f, 100.0f);

	glDisable(GL_BLEND);

	glutSwapBuffers();
}

void reshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	// Setup projection matrix
}

void keyboardFunc(unsigned char key, int x, int y)
{
	unsigned char treeID;
	glReadPixels(x, window_dimensions.y - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &treeID);
	glDisable(GL_STENCIL_TEST);

	switch (key)
	{
	case 'w':
		activeCamera->move(Camera::Direction::forward);
		break;
	case 's':
		activeCamera->move(Camera::Direction::back);
		break;
	case 'a':
		activeCamera->move(Camera::Direction::left);
		break;
	case 'd':
		activeCamera->move(Camera::Direction::right);
		break;
	case 'f':
		lights["flashlight"]->enabled = !lights["flashlight"]->enabled;
		break;
	case 'l':
		lights["lamp"]->enabled = !lights["lamp"]->enabled;
		break;
	case 'm':
		lights["moon"]->enabled = !lights["moon"]->enabled;
		break;
	case 'M':
		fog->enabled = !fog->enabled;
		break;
	case 'r':
		if (treeID == 4)
			inventory->placeOrnament(Inventory::OrnamentType::red, objects["christmasTree"], window_dimensions, x, y, activeCamera);
		if (treeID > 4)
			inventory->placeOrnament(Inventory::OrnamentType::red, treeGenerator->trees[(int)treeID - 5], window_dimensions, x, y, activeCamera);
		break;
	case 'y':
		if (treeID == 4)
			inventory->placeOrnament(Inventory::OrnamentType::yellow, objects["christmasTree"], window_dimensions, x, y, activeCamera);
		if (treeID > 4)
			inventory->placeOrnament(Inventory::OrnamentType::yellow, treeGenerator->trees[(int)treeID - 5], window_dimensions, x, y, activeCamera);
		break;
	case 'b':
		if (treeID == 4)
			inventory->placeOrnament(Inventory::OrnamentType::blue, objects["christmasTree"], window_dimensions, x, y, activeCamera);
		if (treeID > 4)
			inventory->placeOrnament(Inventory::OrnamentType::blue, treeGenerator->trees[(int)treeID - 5], window_dimensions, x, y, activeCamera);
		break;
	case 'h':
		hud->enabled = !hud->enabled;
		break;
	case 27:
		glutLeaveMainLoop();
	}

	lights["flashlight"]->position = glm::vec4(cameras["fps"]->position, 1);
	lights["flashlight"]->spot_direction = cameras["fps"]->center - cameras["fps"]->position;

	glutPostRedisplay();
}

void keyboardSpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		activeCamera->move(Camera::Direction::forward);
		break;
	case GLUT_KEY_DOWN:
		activeCamera->move(Camera::Direction::back);
		break;
	case GLUT_KEY_LEFT:
		activeCamera->move(Camera::Direction::left);
		break;
	case GLUT_KEY_RIGHT:
		activeCamera->move(Camera::Direction::right);
		break;
	case GLUT_KEY_F1:
		activeCamera = cameras["fps"];
		fog->enabled = true;
		break;
	case GLUT_KEY_F2:
		activeCamera = cameras["inside"];
		fog->enabled = true;
		break;
	case GLUT_KEY_F3:
		activeCamera = cameras["outside"];
		fog->enabled = false;
		break;
	}

	lights["flashlight"]->position = glm::vec4(cameras["fps"]->position, 1);
	lights["flashlight"]->spot_direction = cameras["fps"]->center - cameras["fps"]->position;

	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y)
{
	int modifiers = glutGetModifiers();

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		unsigned char ornamentID;
		glReadPixels(x, window_dimensions.y - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &ornamentID);
		glDisable(GL_STENCIL_TEST);

		int ornamentInsertStep = (modifiers == GLUT_ACTIVE_SHIFT) ? 10 : 1;

		switch (ornamentID)
		{
		case 1:
			hud->enabled = true;
			inventory->insertOrnament(Inventory::OrnamentType::red, ornamentInsertStep);
			//cout << "Red: " << inventory->ornamentCount(Inventory::OrnamentType::red) << endl;
			break;
		case 2:
			hud->enabled = true;
			inventory->insertOrnament(Inventory::OrnamentType::yellow, ornamentInsertStep);
			//cout << "Yellow: " << inventory->ornamentCount(Inventory::OrnamentType::yellow) << endl;
			break;
		case 3:
			hud->enabled = true;
			inventory->insertOrnament(Inventory::OrnamentType::blue, ornamentInsertStep);
			//cout << "Blue: " << inventory->ornamentCount(Inventory::OrnamentType::blue) << endl;
			break;
		}
	}
}

void motionFunc(int windowX, int windowY)
{
	activeCamera->look(windowX, windowY, cursor_position, window_dimensions);
	lights["flashlight"]->spot_direction = cameras["fps"]->center - cameras["fps"]->position;
	glutPostRedisplay();
}

void idleFunc()
{
}

void timerFunc(int value)
{
	snowGenerator->update();
	glutTimerFunc(1000.0 / 25.0, timerFunc, 0);
	/*snowflakeTest = spline->point(frame++, frameCount)->position;
	if (frame < frameCount) glutTimerFunc(40, timerFunc, 0);*/
	glutPostRedisplay();
}