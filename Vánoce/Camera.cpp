#include "headers\Camera.h"

Camera::Camera(Config * config,
	float fov, float aspectRatio, float nearPlane, float farPlane,
	glm::vec3 position, glm::vec3 center, glm::vec3 up,
	bool moving, bool looking, bool free, bool animated)
{
	this->config = config;

	this->fov = fov;
	this->aspectRatio = aspectRatio;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

	this->position = position;
	this->center = center;
	this->up = up;

	this->moving = moving;
	this->looking = looking;
	this->free = free;
	this->animated = animated;

	if (animated)
	{
		this->frame = 0;
		this->seconds = config->fOpt("cam_anim_seconds");

		vector<Spline::Point *> points;
		points.push_back(new Spline::Point(config->vOpt("cam_anim_p_start"), config->vOpt("cam_anim_d_start")));
		points.push_back(new Spline::Point(config->vOpt("cam_anim_p1"), config->vOpt("cam_anim_d1")));
		points.push_back(new Spline::Point(config->vOpt("cam_anim_p2"), config->vOpt("cam_anim_d2")));
		points.push_back(new Spline::Point(config->vOpt("cam_anim_p3"), config->vOpt("cam_anim_d3")));
		points.push_back(new Spline::Point(config->vOpt("cam_anim_p4"), config->vOpt("cam_anim_d4")));
		points.push_back(new Spline::Point(config->vOpt("cam_anim_p5"), config->vOpt("cam_anim_d5")));
		points.push_back(new Spline::Point(config->vOpt("cam_anim_p6"), config->vOpt("cam_anim_d6")));
		points.push_back(new Spline::Point(config->vOpt("cam_anim_p_end"), config->vOpt("cam_anim_d_end")));

		this->path = new Spline(config, points);
	}
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

	glm::vec3 direction = config->fOpt("step_fb") * glm::normalize(center - position);
	if (!free) direction.y = 0;

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
		rotation = glm::rotate(glm::mat4(1.0), config->fOpt("step_lr"), up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
		break;
	case right:
		rotation = glm::rotate(glm::mat4(1.0), -config->fOpt("step_lr"), up);
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
		rotation = glm::rotate(glm::mat4(1.0), config->fOpt("step_look"), up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
	}
	else if (movement_direction.x > 0)
	{
		rotation = glm::rotate(glm::mat4(1.0), -config->fOpt("step_look"), up);
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
	}

	glm::vec3 direction = position - center;

	if (movement_direction.y < 0)
	{
		rotation = glm::rotate(glm::mat4(1.0), config->fOpt("step_look"), glm::cross(direction, up));
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
	}
	else if (movement_direction.y > 0)
	{
		rotation = glm::rotate(glm::mat4(1.0), -config->fOpt("step_look"), glm::cross(direction, up));
		newDirection = rotation * glm::vec4(center - position, 1);
		center = position + glm::vec3(newDirection.x, newDirection.y, newDirection.z);
	}
}

void Camera::update()
{
	if (!animated) return;

	int frameCount = seconds * config->fOpt("fps");
	if ((++frame) >= frameCount) frame = 0;

	position = path->point(frame, frameCount)->position;
	center = path->point(frame, frameCount)->direction - position;
}

void Camera::checkBoundaries(Direction direction, glm::vec3 amount)
{
	float min_y = 0;
	float max_y = 100 * config->fOpt("scale");
	float min_xz = -100 * config->fOpt("scale");
	float max_xz = 100 * config->fOpt("scale");

	if (position.x > min_xz && position.x < max_xz && position.z > min_xz && position.z < max_xz &&
		position.y > min_y && position.y < max_y) return;

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
