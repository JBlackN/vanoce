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

	//glm::vec3 sideDirection = glm::normalize(glm::cross(direction, up));
	glm::mat4 rotation;
	glm::vec4 newDirection;

	switch (whereTo)
	{
	case forward:
		position += direction;
		center += direction;
		break;
	case back:
		position -= direction;
		center -= direction;
		break;
	case left:
		//position -= sideDirection;
		//center -= sideDirection;
		rotation = glm::rotate(glm::mat4(1.0), 10.0f, up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
		break;
	case right:
		/*activeCamera->position += sideDirection;
		activeCamera->center += sideDirection;*/
		rotation = glm::rotate(glm::mat4(1.0), -10.0f, up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
		break;
	}
}
