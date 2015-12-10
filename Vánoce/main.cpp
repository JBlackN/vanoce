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
#include "models\crate.h"

const glm::vec2 window_dimensions = glm::vec2(800.0f, 600.0f);
glm::vec2 cursor_position = glm::vec2(0.0f, 0.0f);

map<string, Shader *> shaders;
map<string, Texture *> textures;
map<string, Model *> models;
map<string, Object *> objects;
map<string, Camera *> cameras;

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

	textures["wood"] = new Texture("textures/wood.jpg");
	textures["wall"] = new Texture("textures/wall.jpg");
	textures["stone"] = new Texture("textures/stone.jpg");
	textures["crate"] = new Texture("textures/wooden-crate.jpg");

	// Models

	models["crate"] = new Model(shaders["generic"], textures["crate"], crateNVertices);
	models["crate"]->loadData(crateNAttribsPerVertex, crateNVertices, crateNTriangles, crateVertices, crateTriangles);

	// Objects

	glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(0.2, 0.2, 0.2));
	objects["c1"] = new Object(models["crate"], glm::translate(scale, glm::vec3(0, 0, 5)));
	objects["c2"] = new Object(models["crate"], glm::translate(scale, glm::vec3(-3, 0, 0)));
	objects["c3"] = new Object(models["crate"], glm::translate(scale, glm::vec3(3, 0, 0)));

	// Cameras

	float window_aspectRatio = window_dimensions.x / window_dimensions.y;
	cameras["generic"] = new Camera(50.0f, window_aspectRatio, 0.1f, 100.0f,
		glm::vec3(2, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void displayFunc()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	objects["c1"]->draw(cameras["generic"]);
	objects["c2"]->draw(cameras["generic"]);
	objects["c3"]->draw(cameras["generic"]);

	glutSwapBuffers();
}

void reshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);
	// Setup projection matrix
}

void keyboardFunc(unsigned char key, int x, int y)
{
	Camera * camera = cameras["generic"];
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
	Camera * camera = cameras["generic"];

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

	/*cameras["generic"]->center.x = x*2;
	cameras["generic"]->center.y = -y*2;*/

	//cameras["generic"]->center += ((float)0.01 * glm::normalize(glm::vec3(x, y, 0)));

	glm::vec2 movement_direction = glm::vec2(x - cursor_position.x, y - cursor_position.y);
	cursor_position.x = x;
	cursor_position.y = y;

	if (movement_direction.x > 0 && movement_direction.y > 0)
	{
		cameras["generic"]->center.x += 0.05;
		cameras["generic"]->center.y += 0.05;
	}
	else if (movement_direction.x < 0 && movement_direction.y >= 0)
	{
		cameras["generic"]->center.x -= 0.05;
		cameras["generic"]->center.y += 0.05;
	}
	else if (movement_direction.x < 0 && movement_direction.y < 0)
	{
		cameras["generic"]->center.x -= 0.05;
		cameras["generic"]->center.y -= 0.05;
	}
	else if (movement_direction.x >= 0 && movement_direction.y < 0)
	{
		cameras["generic"]->center.x += 0.05;
		cameras["generic"]->center.y -= 0.05;
	}

	glutPostRedisplay();
}

void idleFunc()
{
}

void timerFunc(int value)
{
}