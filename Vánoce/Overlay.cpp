#include "headers\Overlay.h"

Overlay::Overlay(int winWidth, int winHeight, Shader * shader, Material * material, map<string, Texture*> textures,
	bool enabled, int fps, float seconds, float magnification)
{
	this->enabled = enabled;
	this->fps = fps;
	this->seconds = seconds;

	this->overlayVertices = new float[4*8] {
		0.0f, (float)winHeight, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		(float)winWidth, (float)winHeight, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		(float)winWidth, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f
	};
	this->overlayTriangles = new unsigned int[2*3] {
		0, 1, 2,
		1, 2, 3
	};

	vector<Texture *> textureArray;
	textureArray.push_back(textures["bg"]);
	textureArray.push_back(textures["text"]);

	Model * overlayModel = new Model(shader, material, textureArray, 8, 4, 2, overlayVertices, overlayTriangles);
	this->overlay = new Object(overlayModel, glm::mat4(1), glm::scale(glm::mat4(1), glm::vec3(0.33f, 1, 1)));

	this->magnification = magnification;
	this->offset = 0;
	this->scale = 1;
	this->scaleDirection = descending;
}

Overlay::~Overlay()
{
	delete overlay->model;
	delete overlay;
	delete overlayTriangles;
	delete overlayVertices;
}

void Overlay::draw(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	if (!enabled) return;
	overlay->draw(left, right, bottom, top, nearPlane, farPlane);
}

void Overlay::update()
{
	if (!enabled) return;

	float offsetRange = 1 - 0.33f;
	offset += (offsetRange / ((float)fps * seconds));
	if (offset > offsetRange) offset = 0;

	float scaleRange = 1.0f - (1.0f / glm::sqrt(magnification));
	if (scaleDirection == descending)
		scale -= (scaleRange / ((float)fps * seconds));
	else
		scale += (scaleRange / ((float)fps * seconds));

	if (scale < (1.0f - scaleRange / 2)) scaleDirection = ascending;
	if (scale > 1.0f) scaleDirection = descending;

	overlay->textureMatrix = glm::scale( // "Global" scale
		glm::translate( // "Global" translate
		glm::scale( // "Local" scale
		glm::translate( // "Local " translate
		glm::mat4(1), glm::vec3((1.0f - scale) / 2.0f, (1.0f - scale) / 2.0f, 0)),
		glm::vec3(scale, scale, 1)),
		glm::vec3(offset, 0, 0)),
		glm::vec3(0.33f, 1, 1));
}
