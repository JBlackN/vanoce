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
		checkBoundaries(forward, direction);
		break;
	case back:
		position -= direction;
		center -= direction;
		checkBoundaries(back, direction);
		break;
	case left:
		//position -= sideDirection;
		//center -= sideDirection;
		rotation = glm::rotate(glm::mat4(1.0), 10.0f, up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
		break;
	case right:
		/*position += sideDirection;
		center += sideDirection;*/
		rotation = glm::rotate(glm::mat4(1.0), -10.0f, up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
		break;
	}
}

void Camera::look(int window_x, int window_y, glm::vec2 & cursor_position, glm::vec2 window_dimensions)
{
	if (!looking) return;

	// TODO: Boundaries, scale

	float x = ((float)window_x / window_dimensions.x) - 0.5f;
	float y = ((float)window_y / -window_dimensions.y) + 0.5f;

	/*center.x = x*2;
	center.y = -y*2;*/

	//center += ((float)0.01 * glm::normalize(glm::vec3(x, y, 0)));

	glm::vec2 movement_direction = glm::vec2(x - cursor_position.x, y - cursor_position.y);
	cursor_position.x = x;
	cursor_position.y = y;

	if (movement_direction.x > 0 && movement_direction.y > 0)
	{
		center.x += 0.05;
		center.y += 0.05;
	}
	else if (movement_direction.x < 0 && movement_direction.y >= 0)
	{
		center.x -= 0.05;
		center.y += 0.05;
	}
	else if (movement_direction.x < 0 && movement_direction.y < 0)
	{
		center.x -= 0.05;
		center.y -= 0.05;
	}
	else if (movement_direction.x >= 0 && movement_direction.y < 0)
	{
		center.x += 0.05;
		center.y -= 0.05;
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
