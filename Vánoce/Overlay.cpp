#include "headers\Overlay.h"

Overlay::Overlay(int winWidth, int winHeight, Shader * shader, Material * material, map<string, Texture*> textures,
	glm::mat4 textureAdjustmentMatrix, bool enabled, int fps, float seconds, float magnification)
{
	this->enabled = enabled;
	this->fps = fps;
	this->seconds = seconds;

	this->overlayVertices = new float[4*8] {
		0.0f, 600.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		800.0f, 600.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		800.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f
	};
	this->overlayTriangles = new unsigned int[2*3] {
		0, 1, 2,
		1, 2, 3
	};

	Model * overlayModel = new Model(shader, material, textures["bg"], 8, 4, 2, overlayVertices, overlayTriangles);
	this->overlay = new Object(overlayModel, glm::mat4(1));
	this->text = textures["text"];

	this->textureAdjustmentMatrix = textureAdjustmentMatrix;

	this->magnification = magnification;
	this->offset = 0;
	this->scale = 1;
	this->descending = true;
}

Overlay::~Overlay()
{
}

void Overlay::draw(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	if (!enabled) return;
	overlay->draw(text, left, right, bottom, top, nearPlane, farPlane, textureAdjustmentMatrix);
}

void Overlay::update()
{
	float offsetRange = 1 - 0.33f;
	offset += (offsetRange / ((float)fps * seconds));
	if (offset > offsetRange) offset = 0;

	float scaleRange = 1.0f - (1.0f / glm::sqrt(magnification));
	if (descending)
		scale -= (scaleRange / ((float)fps * seconds));
	else
		scale += (scaleRange / ((float)fps * seconds));

	if (scale < (1.0f - scaleRange / 2)) descending = false;
	if (scale > 1.0f) descending = true;

	textureAdjustmentMatrix = glm::scale( // "Global" scale
		glm::translate( // "Global" translate
		glm::scale( // "Local" scale
		glm::translate( // "Local " translate
		glm::mat4(1), glm::vec3((1.0f - scale) / 2.0f, (1.0f - scale) / 2.0f, 0)),
		glm::vec3(scale, scale, 1)),
		glm::vec3(offset, 0, 0)),
		glm::vec3(0.33f, 1, 1));
}
