#include "headers\Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane, glm::vec3 position, glm::vec3 center, glm::vec3 up)
{
	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

	this->position = position;
	this->center = center;
	this->up = up;
}

Camera::~Camera()
{
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, center, up);
}
