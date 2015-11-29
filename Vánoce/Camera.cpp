#include "headers\Camera.h"

Camera::Camera()
{
	fov = 0.0f;
	aspectRatio = 0.0f;
	nearPlane = 0.0f;
	farPlane = 0.0f;
	position = glm::vec3(-3, -3, -3);
	direction = glm::vec3(0, 0, 0);
	up = glm::vec3(0, 1, 0);
}

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane,
	glm::vec3 position, glm::vec3 direction, glm::vec3 up)
{
	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

	this->position = position;
	this->direction = direction;
	this->up = up;
}

Camera::~Camera()
{
}

glm::mat4 Camera::getCameraMatrix()
{
	glm::mat4 projection = glm::perspective(this->fov, this->aspectRatio, this->nearPlane, this->farPlane);
	glm::mat4 view = glm::lookAt(this->position, this->direction, this->up);

	return projection * view;
}

float Camera::getFov()
{
	return this->fov;
}

void Camera::setFov(float fov)
{
	this->fov = fov;
}

float Camera::getAspectRatio()
{
	return this->aspectRatio;
}

void Camera::setAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}

float Camera::getNearPlane()
{
	return this->nearPlane;
}

void Camera::setNearPlane(float nearPlane)
{
	this->nearPlane = nearPlane;
}

float Camera::getFarPlane()
{
	return this->farPlane;
}

void Camera::setFarPlane(float farPlane)
{
	this->farPlane = farPlane;
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

void Camera::setPosition(glm::vec3 position)
{
	this->position = position;
}

glm::vec3 Camera::getDirection()
{
	return this->direction;
}

void Camera::setDirection(glm::vec3 direction)
{
	this->direction = direction;
}

glm::vec3 Camera::getUp()
{
	return this->up;
}

void Camera::setUp(glm::vec3 up)
{
	this->up = up;
}
