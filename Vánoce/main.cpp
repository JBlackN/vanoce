#include <iostream>
using namespace std;

#include "pgr.h"
#include "headers\Shader.h"
#include "headers\Texture.h"
#include "headers\Model.h"
#include "headers\Object.h"
#include "headers\Camera.h"
#include "models\box.h"

Shader * shader;
Texture * texture;
Model * model;
Object * obj1;
Camera * camera;

GLuint vao, vbo;
GLfloat degRotated = 0.0f;
double lastTime;

glm::vec3 position = glm::vec3(3, 3, 3);
glm::vec3 direction = glm::vec3(-3, -3, -3);
glm::vec3 up = glm::vec3(0, 1, 0);
float viewAngle = 0;

void init(void);
void loadShaders(void);
void loadData(void);
void loadTexture(void);
void loadCamera(void);

void anim_rotate(void);

void displayFunc(void);
void reshapeFunc(int, int);
void keyboardFunc(unsigned char, int, int);
void keyboardSpecialFunc(int, int, int);
void mouseFunc(int, int, int, int);
void idleFunc(void);
void timerFunc(int);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(800, 600);
	glutCreateWindow("Window");

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(keyboardSpecialFunc);
	//glutMouseFunc(mouseFunc);
	glutIdleFunc(idleFunc);
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

	//lastTime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();

	return 0;
}

void init()
{
	shader = new Shader("shaders/generic.vert", "shaders/generic.frag");
	texture = new Texture(shader, "textures/wooden-crate.jpg");
	model = new Model(shader, texture, boxNAttribsPerVertex, boxNVertices, boxNTriangles, boxVertices, boxTriangles);
	obj1 = new Object(model, glm::mat4());
	camera = new Camera(50.0f, 800.0f / 600.0f, 0.1f, 100.0f,
		glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void anim_rotate(int step)
{
	degRotated += step;
	if (degRotated > 360.0f) degRotated -= 360.0f;
}

void displayFunc()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader->getProgram());

	GLint camLoc = glGetUniformLocation(shader->getProgram(), "camera");
	glUniformMatrix4fv(camLoc, 1, GL_FALSE, glm::value_ptr(camera->getCameraMatrix()));
	cout << camera->getPosition().x << endl;

	glUseProgram(0);
	
	obj1->drawObject();
	
	glutSwapBuffers();
}

void reshapeFunc(int width, int height)
{
}

void keyboardFunc(unsigned char key, int x, int y)
{
	glm::vec3 v;
	glm::vec4 a;
	glm::mat4 rotation;

	switch (key)
	{
	case 'w':
		camera->setPosition(camera->getPosition() + glm::normalize(camera->getDirection()));
		glutPostRedisplay();
		break;
	case 's':
		camera->setPosition(camera->getPosition() - glm::normalize(camera->getDirection()));
		glutPostRedisplay();
		break;
	case 'a':
		rotation = glm::rotate(glm::mat4(1.0), 1.0f, camera->getUp());
		a = rotation * glm::vec4(camera->getDirection(), 1);
		camera->setDirection(glm::vec3(a.x, a.y, a.z));
		glutPostRedisplay();
		break;
	case 'd':
		rotation = glm::rotate(glm::mat4(1.0), -1.0f, camera->getUp());
		a = rotation * glm::vec4(camera->getDirection(), 1);
		camera->setDirection(glm::vec3(a.x, a.y, a.z));
		glutPostRedisplay();
		break;
	}
}

void keyboardSpecialFunc(int key, int x, int y)
{
	glm::mat4 view;
	GLint viewLoc = glGetUniformLocation(shader->getProgram(), "view");

	switch (key)
	{
	case GLUT_KEY_F1:
		camera->setPosition(glm::vec3(3, 3, 3));
		camera->setDirection(glm::vec3(-3, -3, -3));
		glutPostRedisplay();
		break;
	case GLUT_KEY_F2:
		camera->setPosition(glm::vec3(10, 0, 0));
		camera->setDirection(glm::vec3(-10, 0, 0));
		glutPostRedisplay();
		//view = glm::lookAt(glm::vec3(10, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case GLUT_KEY_F3:
		camera->setPosition(glm::vec3(0, -5, -5));
		camera->setDirection(glm::vec3(0, 5, 5));
		glutPostRedisplay();
		//view = glm::lookAt(glm::vec3(0, -5, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	}
}

void mouseFunc(int button, int state, int x, int y)
{
}

void idleFunc()
{
}

void timerFunc(int value)
{
	glutTimerFunc(10, timerFunc, 1);
	anim_rotate(value);
	glutPostRedisplay();
}