#include <iostream>
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

#include "models\skybox.h"
#include "models\terrain.h"
#include "models\home.h"
#include "models\table.h"
#include "models\chair.h"
#include "models\carton.h"
#include "models\ornament.h"
#include "models\stand.h"
#include "models\tree.h"

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

void init(void);

void displayFunc(void);
void reshapeFunc(int, int);
void keyboardFunc(unsigned char, int, int);
void keyboardSpecialFunc(int, int, int);
void mouseFunc(int, int, int, int);
void passiveMotionFunc(int, int);
void idleFunc(void);
void timerFunc(int);

int main(int argc, char **argv)
{
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
	glutPassiveMotionFunc(passiveMotionFunc); // TODO
	//glutIdleFunc(idleFunc);
	//glutTimerFunc(10, timerFunc, 1);

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

	// Materials

	materials["skybox"] = new Material(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), 0);
	materials["snow"] = new Material(glm::vec3(0, 0, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 10);
	materials["home"] = new Material(glm::vec3(0, 0, 0.4f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 1);
	materials["wood"] = new Material(glm::vec3(0, 0, 0.2f), glm::vec3(0.6f, 0.4f, 0.4f), glm::vec3(0.8f, 0.6f, 0.4f), 1);
	materials["fabric"] = new Material(glm::vec3(0, 0.2f, 0.4f), glm::vec3(1, 1, 1), glm::vec3(1, 1, 1), 1);
	materials["cardboard"] = new Material(glm::vec3(0.2f, 0.2f, 0), glm::vec3(0.6f, 0.4f, 0.2f), glm::vec3(0.8f, 0.6f, 0.4f), 5);
	materials["ornament_red"] = new Material(glm::vec3(0.2f, 0, 0.2f), glm::vec3(0.7f, 0, 0.2f), glm::vec3(0.6f, 0.2f, 0.1f), 51.2f);
	materials["ornament_yellow"] = new Material(glm::vec3(0.6f, 0.5f, 0), glm::vec3(1, 0.9f, 0), glm::vec3(1, 1, 0.4f), 51.2f);
	materials["ornament_blue"] = new Material(glm::vec3(0.2f, 0.2f, 0.4f), glm::vec3(0.2f, 0.2f, 1), glm::vec3(0.2f, 0.6f, 1), 51.2f);
	materials["metal"] = new Material(glm::vec3(0, 0, 0.2f), glm::vec3(0, 0.1f, 0.3f), glm::vec3(1, 1, 1), 15);
	materials["tree"] = new Material(glm::vec3(0, 0.2f, 0), glm::vec3(0.2f, 0.4f, 0.2f), glm::vec3(0.2f, 0.4f, 0.4f), 1);

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

	objects["christmasTree"] = new Object(models["tree"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(1.5f, 1.4f, -1.5f)));
	treeGenerator = new TreeGenerator(models["tree"]);
	treeGenerator->generateTrees(50, 19);

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

	glDisable(GL_STENCIL_TEST);

	treeGenerator->drawTrees(activeCamera, lights, fog);

	glutSwapBuffers();
}

void reshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	// Setup projection matrix
}

void keyboardFunc(unsigned char key, int x, int y)
{
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
	if ((button == GLUT_LEFT_BUTTON) && (state = GLUT_UP))
	{
		unsigned char ornamentID;
		glReadPixels(x, window_dimensions.y - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &ornamentID);
		glDisable(GL_STENCIL_TEST);
		if (ornamentID == 0)
			cout << "BG" << endl;
		else
		{
			switch (ornamentID)
			{
			case 1:
				cout << "Red" << endl;
				break;
			case 2:
				cout << "Yellow" << endl;
				break;
			case 3:
				cout << "Blue" << endl;
				break;
			case 4:
				cout << "Christmas tree" << endl;
				break;
			}
		}
	}
}

void passiveMotionFunc(int windowX, int windowY)
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
}