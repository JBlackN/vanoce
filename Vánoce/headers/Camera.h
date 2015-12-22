#pragma once

#include <cstdlib>
#include <vector>
using namespace std;

#include "pgr.h"
#include"Config.h"
#include "Spline.h"

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
	bool animated;

	Spline * path;
	int frame;
	int seconds;

	enum Direction { forward, back, left, right };
private:
	Config * config;
public:
	Camera(Config * config,
		float fov, float aspectRatio, float nearPlane, float farPlane,
		glm::vec3 position, glm::vec3 center, glm::vec3 up,
		bool moving = false, bool looking = false, bool animated = false);
	~Camera();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void move(Direction whereTo);
	void look(int window_x, int window_y, glm::vec2 & cursor_position, glm::vec2 window_dimensions);
	void update();
private:
	void checkBoundaries(Direction direction, glm::vec3 amount);
};

