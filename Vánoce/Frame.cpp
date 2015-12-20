#include "headers\Frame.h"

Frame::Frame(Model * frameModel, glm::mat4 matrix, int fps, float seconds)
{
	this->frame = new Object(frameModel, matrix);
	this->fps = fps;
	this->seconds = seconds;

	this->textureAdjustmentMatrix = glm::scale(glm::translate(glm::mat4(1), glm::vec3(0, 0, 0)),
		glm::vec3(1.0f / 4.0f, 1.0f / 2.0f, 1));
	this->frameCounter = 1;
	this->currentPicture = 0;
}

Frame::~Frame()
{
}

void Frame::draw(Camera * camera, map<string, Light*> lights, Fog * fog)
{
	frame->draw(camera, lights, fog, textureAdjustmentMatrix);
}

void Frame::update()
{
	if (++frameCounter == seconds * fps)
	{
		currentPicture++;
		frameCounter = 0;
	}
	if (currentPicture > 7) currentPicture = 0;

	this->textureAdjustmentMatrix = glm::scale(glm::translate(glm::mat4(1),
		glm::vec3((float)(currentPicture % 4) / 4.0f, currentPicture < 4 ? 0 : (1.0f / 2.0f), 0)),
		glm::vec3(1.0f / 4.0f, 1.0f / 2.0f, 1));
}
