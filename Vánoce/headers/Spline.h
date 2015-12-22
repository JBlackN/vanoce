#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

#include "pgr.h"
#include "Config.h"

class Spline
{
public:
	struct Point {
		glm::vec3 position;
		glm::vec3 direction;

		Point(glm::vec3 position, glm::vec3 direction)
		{
			this->position = position;
			this->direction = direction;
		}
	};
private:
	Config * config;
	vector<Point *> points;
public:
	Spline(Config * config, glm::vec3 start_position, int pointCount, float pointDirectionAngle = 90.0f);
	Spline(Config * config, vector<Point *> points);
	~Spline();

	Point * point(int frame, int frameCount);
private:
	glm::vec3 pointPosition(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t);
	glm::vec3 pointDirection(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t);
};

