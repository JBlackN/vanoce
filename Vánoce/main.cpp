#include <iostream>
#include <string>
#include <map>
using namespace std;

#include "pgr.h"
#include "headers\Shader.h"
#include "headers\Texture.h"
#include "headers\Model.h"
#include "headers\Object.h"
#include "headers\Camera.h"

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
Camera * camera;

map<string, Shader *> shaders;
map<string, Texture *> textures;
map<string, Model *> models;
map<string, Object *> objects;

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
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

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
	/*loadShaders();
	loadTexture();
	loadData();
	loadCamera();*/

	glutMainLoop();

	return 0;
}

void init()
{
	// Shaders

	shaders["generic"] = new Shader("shaders/generic.vert", "shaders/generic.frag");

	//Textures

	textures["snow"] = new Texture("textures/snow.png");
	textures["home"] = new Texture("textures/home.png");
	textures["wood"] = new Texture("textures/wood.jpg");
	textures["leather"] = new Texture("textures/leather.png");
	textures["fabric"] = new Texture("textures/fabric.png");
	textures["cardboard"] = new Texture("textures/cardboard.jpg");
	textures["glass"] = new Texture("textures/glass.png");
	textures["metal"] = new Texture("textures/metal.png");
	textures["tree"] = new Texture("textures/tree.png");

	// Models

	models["terrain"] = new Model(shaders["generic"], textures["snow"], terrainNTriangles);
	models["terrain"]->loadData(terrainNAttribsPerVertex, terrainNVertices, terrainNTriangles, terrainVertices, terrainTriangles);
	models["home"] = new Model(shaders["generic"], textures["home"], homeNTriangles);
	models["home"]->loadData(homeNAttribsPerVertex, homeNVertices, homeNTriangles, homeVertices, homeTriangles);
	models["table"] = new Model(shaders["generic"], textures["wood"], tableNTriangles);
	models["table"]->loadData(tableNAttribsPerVertex, tableNVertices, tableNTriangles, tableVertices, tableTriangles);
	models["chair"] = new Model(shaders["generic"], textures["fabric"], chairNTriangles);
	models["chair"]->loadData(chairNAttribsPerVertex, chairNVertices, chairNTriangles, chairVertices, chairTriangles);
	models["carton"] = new Model(shaders["generic"], textures["cardboard"], cartonNTriangles);
	models["carton"]->loadData(cartonNAttribsPerVertex, cartonNVertices, cartonNTriangles, cartonVertices, cartonTriangles);
	models["ornament"] = new Model(shaders["generic"], textures["glass"], ornamentNTriangles);
	models["ornament"]->loadData(ornamentNAttribsPerVertex, ornamentNVertices, ornamentNTriangles, ornamentVertices, ornamentTriangles);
	models["stand"] = new Model(shaders["generic"], textures["metal"], standNTriangles);
	models["stand"]->loadData(standNAttribsPerVertex, standNVertices, standNTriangles, standVertices, standTriangles);
	models["tree"] = new Model(shaders["generic"], textures["tree"], treeNTriangles);
	models["tree"]->loadData(treeNAttribsPerVertex, treeNVertices, treeNTriangles, treeVertices, treeTriangles);

	// Objects

	objects["terrain"] = new Object(models["terrain"], glm::scale(glm::rotate(glm::mat4(), 90.0f, glm::vec3(-1, 0, 0)),
		glm::vec3(100, 100, 1)));
	objects["home"] = new Object(models["home"], glm::scale(glm::mat4(), glm::vec3(5, 5, 5)));
	objects["table"] = new Object(models["table"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(-1.5f, 0.5f, 0)));
	objects["chair"] = new Object(models["chair"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(0, 0.5f, 0)));
	objects["carton"] = new Object(models["carton"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 3, 5)),
		glm::vec3(-0.9f, 1.75f, -0.6f)));

	objects["ornament1"] = new Object(models["ornament"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(-0.9f, 1.0f, -0.55f)));
	objects["ornament2"] = new Object(models["ornament"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(-1.0f, 1.0f, -0.55f)));
	objects["ornament3"] = new Object(models["ornament"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(-0.97f, 1.0f, -0.67f)));

	objects["stand"] = new Object(models["stand"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(1.5f, 0.2f, -1.5f)));

	objects["tree1"] = new Object(models["tree"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(1.5f, 1.4f, -1.5f)));
	objects["tree2"] = new Object(models["tree"], glm::translate(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)),
		glm::vec3(5.0f, 1.2f, -5.0f)));

	// Cameras

	float window_aspectRatio = window_dimensions.x / window_dimensions.y;
	camera = new Camera(50.0f, window_aspectRatio, 0.1f, 1000.0f,
		glm::vec3(2, 7, -50), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0));
}

void displayFunc()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objects["terrain"]->draw(camera);
	objects["home"]->draw(camera);
	objects["table"]->draw(camera);
	objects["chair"]->draw(camera);
	objects["carton"]->draw(camera);

	objects["ornament1"]->draw(camera);
	objects["ornament2"]->draw(camera);
	objects["ornament3"]->draw(camera);

	objects["stand"]->draw(camera);

	objects["tree1"]->draw(camera);
	objects["tree2"]->draw(camera);

	glutSwapBuffers();
}

void reshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	// Setup projection matrix
}

void keyboardFunc(unsigned char key, int x, int y)
{
	glm::vec3 direction = glm::normalize(camera->center - camera->position);
	direction.y = 0;

	glm::vec3 sideDirection = glm::normalize(glm::cross(direction, camera->up));
	glm::vec4 a;
	glm::mat4 rotation;

	switch (key)
	{
	case 'w':
		camera->position += direction;
		camera->center += direction;
		break;
	case 's':
		camera->position -= direction;
		camera->center -= direction;
		break;
	case 'a':
		/*camera->position -= sideDirection;
		camera->center -= sideDirection;*/
		rotation = glm::rotate(glm::mat4(1.0), 10.0f, camera->up);
		a = rotation * glm::vec4(camera->center - camera->position, 1);
		camera->center = camera->position + glm::vec3(a.x, a.y, a.z);
		break;
	case 'd':
		/*camera->position += sideDirection;
		camera->center += sideDirection;*/
		rotation = glm::rotate(glm::mat4(1.0), -10.0f, camera->up);
		a = rotation * glm::vec4(camera->center - camera->position, 1);
		camera->center = camera->position + glm::vec3(a.x, a.y, a.z);
		break;
	case 27:
		glutLeaveMainLoop();
	}

	glutPostRedisplay();
}

void keyboardSpecialFunc(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		camera->position = glm::vec3(3, 3, 3);
		camera->center = glm::vec3(0, 0, 0);
		camera->up = glm::vec3(0, 1, 0);
		break;
	case GLUT_KEY_F2:
		camera->position = glm::vec3(3, 0, 3);
		camera->center = glm::vec3(0, 0, 0);
		camera->up = glm::vec3(0, 1, 0);
		break;
	case GLUT_KEY_F3:
		camera->position = glm::vec3(0, 0, -3);
		camera->center = glm::vec3(0, 0, 0);
		camera->up = glm::vec3(0, 1, 0);
		break;
	}

	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y)
{
}

void passiveMotionFunc(int windowX, int windowY)
{
	// TODO: Boundaries, scale
	
	float x = ((float)windowX / window_dimensions.x) - 0.5f;
	float y = ((float)windowY / -window_dimensions.y) + 0.5f;

	/*camera->center.x = x*2;
	camera->center.y = -y*2;*/

	//camera->center += ((float)0.01 * glm::normalize(glm::vec3(x, y, 0)));

	glm::vec2 movement_direction = glm::vec2(x - cursor_position.x, y - cursor_position.y);
	cursor_position.x = x;
	cursor_position.y = y;

	if (movement_direction.x > 0 && movement_direction.y > 0)
	{
		camera->center.x += 0.05;
		camera->center.y += 0.05;
	}
	else if (movement_direction.x < 0 && movement_direction.y >= 0)
	{
		camera->center.x -= 0.05;
		camera->center.y += 0.05;
	}
	else if (movement_direction.x < 0 && movement_direction.y < 0)
	{
		camera->center.x -= 0.05;
		camera->center.y -= 0.05;
	}
	else if (movement_direction.x >= 0 && movement_direction.y < 0)
	{
		camera->center.x += 0.05;
		camera->center.y -= 0.05;
	}

	glutPostRedisplay();
}

void idleFunc()
{
}

void timerFunc(int value)
{
}