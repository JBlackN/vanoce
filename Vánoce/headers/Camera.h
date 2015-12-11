#pragma once

#include "pgr.h"

class Camera
{
public:
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	glm::vec3 position;
	glm::vec3 center;
	glm::vec3 up;

	bool moving;
	bool looking;

	enum Direction { forward, back, left, right };

	Camera(float fov, float aspectRatio, float nearPlane, float farPlane,
		glm::vec3 position, glm::vec3 center, glm::vec3 up,
		bool moving = false, bool looking = false);
	~Camera();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void move(Direction whereTo);
};

