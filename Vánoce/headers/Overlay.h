#pragma once

#include <string>
#include <map>
using namespace std;

#include "pgr.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "Model.h"
#include "Object.h"

class Overlay
{
public:
	bool enabled;
	int fps;
	float seconds;
private:
	float * overlayVertices;
	unsigned int * overlayTriangles;

	Object * overlay;

	float magnification;
	float offset;
	float scale;
	enum ScaleDirection {ascending, descending} scaleDirection;
public:
	Overlay(int winWidth, int winHeight, Shader * shader, Material * material, map<string, Texture *> textures,
		bool enabled = false, int fps = 25, float seconds = 5.0f, float magnification = 25.0f);
	~Overlay();

	void draw(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 100.0f);
	void update();
};
