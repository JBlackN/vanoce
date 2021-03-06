//----------------------------------------------------------------------------------------
/**
* \file       main.cpp
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains main function and GLUT callbacks.
*
*  Functions in this file create a window using GLUT and register callbacks. Scene is
*  created and drawn in the window. Callbacks call scene's methods.
*
*/
//----------------------------------------------------------------------------------------
#include <iostream>
#include <ctime>
#include <string>
#include <map>
using namespace std;

#include "pgr.h"
#include "headers\Scene.h"

glm::vec2 window_dimensions = glm::vec2(800.0f, 600.0f); ///< Initial window dimensions.
glm::vec2 cursor_position = glm::vec2(0.0f, 0.0f); ///< Initial last cursor position.

Scene * scene; ///< Holds and manages entire scene.

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
	glutCreateWindow("V�noce");

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(keyboardSpecialFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);

	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	scene = new Scene(window_dimensions, timerFunc);

	glutMainLoop();

	delete scene;

	return 0;
}

void displayFunc()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	scene->draw(window_dimensions);

	glutSwapBuffers();
}

void reshapeFunc(int width, int height)
{
	glViewport(0, 0, width, height);

	window_dimensions = glm::vec2(width, height);
	scene->reshape(window_dimensions);
}

void keyboardFunc(unsigned char key, int x, int y)
{
	unsigned char treeID;
	glReadPixels(x, window_dimensions.y - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &treeID);
	glDisable(GL_STENCIL_TEST);

	switch (key)
	{
	case 'w':
		scene->activeCamera->move(Camera::Direction::forward);
		break;
	case 's':
		scene->activeCamera->move(Camera::Direction::back);
		break;
	case 'a':
		scene->activeCamera->move(Camera::Direction::left);
		break;
	case 'd':
		scene->activeCamera->move(Camera::Direction::right);
		break;
	case 'f':
		scene->lights["flashlight"]->enabled = !scene->lights["flashlight"]->enabled;
		break;
	case 'l':
		scene->lights["lamp"]->enabled = !scene->lights["lamp"]->enabled;
		break;
	case 'm':
		scene->lights["moon"]->enabled = !scene->lights["moon"]->enabled;
		break;
	case 'M':
		scene->fog->enabled = !scene->fog->enabled;
		break;
	case 'R':
		scene->reload(window_dimensions);
		break;
	case 'r':
		if (treeID == 5)
			scene->inventory->placeOrnament(Inventory::OrnamentType::red,
				scene->objects["christmasTree"], window_dimensions, x, y, scene->activeCamera);
		if (treeID - 6 > scene->treeGenerator->trees.size() - 1) break;
		if (treeID > 5)
			scene->inventory->placeOrnament(Inventory::OrnamentType::red,
				scene->treeGenerator->trees[(int)treeID - 6], window_dimensions, x, y, scene->activeCamera);
		break;
	case 'y':
		if (treeID == 5)
			scene->inventory->placeOrnament(Inventory::OrnamentType::yellow,
				scene->objects["christmasTree"], window_dimensions, x, y, scene->activeCamera);
		if (treeID - 6 > scene->treeGenerator->trees.size() - 1) break;
		if (treeID > 5)
			scene->inventory->placeOrnament(Inventory::OrnamentType::yellow,
				scene->treeGenerator->trees[(int)treeID - 6], window_dimensions, x, y, scene->activeCamera);
		break;
	case 'b':
		if (treeID == 5)
			scene->inventory->placeOrnament(Inventory::OrnamentType::blue,
				scene->objects["christmasTree"], window_dimensions, x, y, scene->activeCamera);
		if (treeID - 6 > scene->treeGenerator->trees.size() - 1) break;
		if (treeID > 5)
			scene->inventory->placeOrnament(Inventory::OrnamentType::blue,
				scene->treeGenerator->trees[(int)treeID - 6], window_dimensions, x, y, scene->activeCamera);
		break;
	case 'h':
		scene->hud->enabled = !scene->hud->enabled;
		break;
	case 'S':
		if (scene->snowGenerator->isEnabled()) scene->snowGenerator->disable();
		else scene->snowGenerator->enable();
		break;
	case 27:
		glutLeaveMainLoop();
	}

	scene->lights["flashlight"]->position = glm::vec4(scene->cameras["fps"]->position, 1);
	scene->lights["flashlight"]->spot_direction = scene->cameras["fps"]->center - scene->cameras["fps"]->position;

	glutPostRedisplay();
}

void keyboardSpecialFunc(int key, int x, int y)
{
	Camera * lastCamera = NULL;

	switch (key)
	{
	case GLUT_KEY_UP:
		scene->activeCamera->move(Camera::Direction::forward);
		break;
	case GLUT_KEY_DOWN:
		scene->activeCamera->move(Camera::Direction::back);
		break;
	case GLUT_KEY_LEFT:
		scene->activeCamera->move(Camera::Direction::left);
		break;
	case GLUT_KEY_RIGHT:
		scene->activeCamera->move(Camera::Direction::right);
		break;
	case GLUT_KEY_F1:
		lastCamera = scene->activeCamera;
		scene->activeCamera = scene->cameras["fps"];
		scene->fog->enabled = true;
		break;
	case GLUT_KEY_F2:
		lastCamera = scene->activeCamera;
		scene->activeCamera = scene->cameras["inside"];
		scene->fog->enabled = true;
		break;
	case GLUT_KEY_F3:
		lastCamera = scene->activeCamera;
		scene->activeCamera = scene->cameras["outside"];
		scene->fog->enabled = false;
		break;
	case GLUT_KEY_F4:
		lastCamera = scene->activeCamera;
		scene->activeCamera = scene->cameras["animated"];
		scene->fog->enabled = true;
		break;
	case GLUT_KEY_F5:
		lastCamera = scene->activeCamera;
		scene->cameras["free"]->fov = scene->activeCamera->fov;
		scene->cameras["free"]->aspectRatio = scene->activeCamera->aspectRatio;
		scene->cameras["free"]->nearPlane = scene->activeCamera->nearPlane;
		scene->cameras["free"]->farPlane = scene->activeCamera->farPlane;
		scene->cameras["free"]->position = scene->activeCamera->position;
		scene->cameras["free"]->center = scene->activeCamera->center;
		scene->activeCamera = scene->cameras["free"];
		scene->fog->enabled = true;
		break;
	}

	if (lastCamera == scene->cameras["fps"])
	{
		scene->lights["flashlight"]->position = glm::vec4(scene->cameras["fps"]->position, 1);
		scene->lights["flashlight"]->spot_direction = scene->cameras["fps"]->center - scene->cameras["fps"]->position;
	}
	else if (lastCamera == scene->cameras["free"])
	{
		scene->lights["flashlight"]->position = glm::vec4(scene->cameras["free"]->position, 1);
		scene->lights["flashlight"]->spot_direction = scene->cameras["free"]->center - scene->cameras["free"]->position;
	}

	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y)
{
	int modifiers = glutGetModifiers();

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		if (scene->overlay->enabled) scene->overlay->enabled = false;

		unsigned char ornamentID;
		glReadPixels(x, window_dimensions.y - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &ornamentID);
		glDisable(GL_STENCIL_TEST);

		int ornamentInsertStep = (modifiers == GLUT_ACTIVE_SHIFT) ? 10 : 1;

		switch (ornamentID)
		{
		case 1:
			scene->hud->enabled = true;
			scene->inventory->insertOrnament(Inventory::OrnamentType::red, ornamentInsertStep);
			break;
		case 2:
			scene->hud->enabled = true;
			scene->inventory->insertOrnament(Inventory::OrnamentType::yellow, ornamentInsertStep);
			break;
		case 3:
			scene->hud->enabled = true;
			scene->inventory->insertOrnament(Inventory::OrnamentType::blue, ornamentInsertStep);
			break;
		case 4:
			scene->overlay->enabled = true;
			break;
		}
	}
}

void motionFunc(int windowX, int windowY)
{
	scene->activeCamera->look(windowX, windowY, cursor_position, window_dimensions);
	scene->lights["flashlight"]->spot_direction = scene->cameras["fps"]->center - scene->cameras["fps"]->position;
	glutPostRedisplay();
}

void idleFunc()
{
}

void timerFunc(int value)
{
	if (scene->activeCamera == scene->cameras["animated"]) scene->activeCamera->update();

	scene->snowGenerator->update();
	scene->overlay->update();
	scene->frame->update();
	glutTimerFunc(1000.0 / scene->config->fOpt("fps"), timerFunc, 0);
	glutPostRedisplay();
}