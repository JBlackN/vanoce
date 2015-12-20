#include "headers\Camera.h"

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane,
	glm::vec3 position, glm::vec3 center, glm::vec3 up,
	bool moving, bool looking)
{
	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

	this->position = position;
	this->center = center;
	this->up = up;

	this->moving = moving;
	this->looking = looking;
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

void Camera::move(Direction whereTo)
{
	if (!moving) return;

	glm::vec3 direction = glm::normalize(center - position);
	direction.y = 0;

	glm::mat4 rotation;
	glm::vec4 newDirection;

	switch (whereTo)
	{
	case forward:
		position += direction;
		center += direction;
		checkBoundaries(forward, direction);
		break;
	case back:
		position -= direction;
		center -= direction;
		checkBoundaries(back, direction);
		break;
	case left:
		rotation = glm::rotate(glm::mat4(1.0), 10.0f, up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
		break;
	case right:
		rotation = glm::rotate(glm::mat4(1.0), -10.0f, up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
		break;
	}
}

void Camera::look(int window_x, int window_y, glm::vec2 & cursor_position, glm::vec2 window_dimensions)
{
	if (!looking) return;

	float x = ((float)window_x / window_dimensions.x) - 0.5f;
	float y = ((float)window_y / -window_dimensions.y) + 0.5f;

	glm::vec2 movement_direction = glm::vec2(x - cursor_position.x, y - cursor_position.y);
	cursor_position.x = x;
	cursor_position.y = y;

	glm::mat4 rotation;
	glm::vec4 newDirection;

	if (movement_direction.x < 0)
	{
		rotation = glm::rotate(glm::mat4(1.0), 2.0f, up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
	}
	else if (movement_direction.x > 0)
	{
		rotation = glm::rotate(glm::mat4(1.0), -2.0f, up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
	}

	glm::vec3 direction = position - center;

	if (movement_direction.y < 0)
	{
		rotation = glm::rotate(glm::mat4(1.0), 2.0f, glm::cross(direction, up));
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
	}
	else if (movement_direction.y > 0)
	{
		rotation = glm::rotate(glm::mat4(1.0), -2.0f, glm::cross(direction, up));
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
	}
}

void Camera::checkBoundaries(Direction direction, glm::vec3 amount)
{
	if (position.x > -100 && position.x < 100 && position.z > -100 && position.z < 100) return;

	switch (direction)
	{
	case forward:
		position -= amount;
		center -= amount;
		break;
	case back:
		position += amount;
		center += amount;
		break;
	}
}
