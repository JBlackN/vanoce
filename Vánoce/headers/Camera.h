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

	Camera(float fov, float aspectRatio, float nearPlane, float farPlane, glm::vec3 position, glm::vec3 center, glm::vec3 up);
	~Camera();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
};

