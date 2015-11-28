#include <iostream>
using namespace std;

#include "pgr.h"
#include "headers\Shader.h"
#include "headers\Texture.h"

Shader * shader;
Texture * texture;

GLuint vao, vbo;
GLfloat degRotated = 0.0f;
double lastTime;

glm::vec3 position = glm::vec3(3, 3, 3);
glm::vec3 direction = glm::vec3(-3, -3, -3);
glm::vec3 up = glm::vec3(0, 1, 0);
float viewAngle = 0;

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
	glutMouseFunc(mouseFunc);
	glutIdleFunc(idleFunc);
	glutTimerFunc(10, timerFunc, 1);

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	loadShaders();
	loadTexture();
	loadData();
	loadCamera();

	lastTime = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();

	return 0;
}

void loadShaders()
{
	shader = new Shader("shaders/generic.vert", "shaders/generic.frag");
	/*GLuint vertexShader = pgr::createShaderFromFile(GL_VERTEX_SHADER, "shaders/generic.vert");
	GLuint fragmentShader = pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/generic.frag");

	GLint status;
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) cerr << "Vertex shader compile error.";
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) cerr << "Fragment shader compile error.";

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);*/
}

void loadData()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	GLfloat vertexData[] = {
		// bottom
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

		// top
		-1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

		// front
		-1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
		1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

		// back
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

		// left
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

		// right
		1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,   0.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	GLint positionLoc = glGetAttribLocation(shader->getProgram(), "position");
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);

	GLint vTexCoordLoc = glGetAttribLocation(shader->getProgram(), "vTexCoord");
	glEnableVertexAttribArray(vTexCoordLoc);
	glVertexAttribPointer(vTexCoordLoc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void loadTexture()
{
	texture = new Texture(shader, "textures/wooden-crate.jpg");
	/*tex = pgr::createTexture("textures/wooden-crate.jpg");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);*/
}

void loadCamera()
{
	glUseProgram(shader->getProgram());

	glm::mat4 projection = glm::perspective(50.0f, 800.0f / 600.0f, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(shader->getProgram(), "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glm::vec3 center = position + direction;

	glm::mat4 view = glm::lookAt(position, center, up);
	GLint viewLoc = glGetUniformLocation(shader->getProgram(), "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glUseProgram(0);
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

	GLint modelLoc = glGetUniformLocation(shader->getProgram(), "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::rotate(glm::mat4(), degRotated, glm::vec3(0.0f, 1.0f, 0.0f))));

	glBindVertexArray(vao);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getTexture());

	GLint texLoc = glGetUniformLocation(shader->getProgram(), "tex");
	glUniform1i(texLoc, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6*2*3);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);

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
		position += glm::normalize(direction);
		loadCamera();
		glutPostRedisplay();
		break;
	case 's':
		position -= glm::normalize(direction);
		loadCamera();
		glutPostRedisplay();
		break;
	case 'a':
		rotation = glm::rotate(glm::mat4(1.0), 1.0f, up);
		a = rotation * glm::vec4(direction, 1);
		direction = glm::vec3(a.x, a.y, a.z);
		loadCamera();
		glutPostRedisplay();
		break;
	case 'd':
		rotation = glm::rotate(glm::mat4(1.0), -1.0f, up);
		a = rotation * glm::vec4(direction, 1);
		direction = glm::vec3(a.x, a.y, a.z);
		loadCamera();
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
		position = glm::vec3(3, 3, 3);
		direction = glm::vec3(-3, -3, -3);
		loadCamera();
		glutPostRedisplay();
		break;
	case GLUT_KEY_F2:
		position = glm::vec3(10, 0, 0);
		direction = glm::vec3(-10, 0, 0);
		loadCamera();
		glutPostRedisplay();
		//view = glm::lookAt(glm::vec3(10, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		break;
	case GLUT_KEY_F3:
		position = glm::vec3(0, -5, -5);
		direction = glm::vec3(0, 5, 5);
		loadCamera();
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