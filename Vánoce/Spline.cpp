#include "headers\Spline.h"

Spline::Spline(glm::vec3 start_position, int pointCount, float pointDirectionAngle) // TODO: Use of pointDirAngle (not needed)
{
	this->points.push_back(new Point(glm::vec3(start_position.x, start_position.y + 10, start_position.z), glm::vec3(0)));

	float height = start_position.y;
	this->points.push_back(new Point(start_position, 
		glm::normalize(glm::vec3(start_position.x, -start_position.y, start_position.z))));

	for (int i = 1; i < pointCount; i++)
	{
		glm::vec3 position = glm::vec3(rand() % 38 - 19, (pointCount - 1 - i) * (height / ((float)pointCount - 1.0f)), rand() % 38 - 19);
		glm::vec3 direction = glm::normalize(glm::vec3(rand() % 38 - 19 - position.x, -1 - position.y, rand() % 38 - 19 - position.z));

		this->points.push_back(new Point(position, direction));
	}

	this->points.push_back(new Point(glm::vec3(points.back()->position.x, -10, points.back()->position.z), glm::vec3(0)));
}

Spline::~Spline()
{
}

Spline::Point * Spline::point(int frame, int frameCount)
{
	float framesPerSegment = (float)frameCount / (points.size() - 3);
	int segmentID = floor((float)frame / framesPerSegment);

	glm::vec3 position = pointPosition(
		points[segmentID]->position,
		points[segmentID + 1]->position,
		points[segmentID + 2]->position,
		points[segmentID + 3]->position,
		(frame - (segmentID * framesPerSegment)) / framesPerSegment);

	glm::vec3 direction = pointDirection(
		points[segmentID]->position,
		points[segmentID + 1]->position,
		points[segmentID + 2]->position,
		points[segmentID + 3]->position,
		(frame - (segmentID * framesPerSegment)) / framesPerSegment);

	return new Point(position, direction);
}

glm::vec3 Spline::pointPosition(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t)
{
	glm::vec4 r1 = (1.0f / 2.0f) * glm::vec4(t*t*t, t*t, t, 1);
	glm::vec4 r2 = r1 * glm::mat4(
		glm::vec4(-1, 2, -1, 0),
		glm::vec4(3, -5, 0, 2),
		glm::vec4(-3, 4, 1, 0),
		glm::vec4(1, -1, 0, 0)
		);
	return glm::vec3(
		r2.x * p0.x + r2.y * p1.x + r2.z * p2.x + r2.w * p3.x,
		r2.x * p0.y + r2.y * p1.y + r2.z * p2.y + r2.w * p3.y,
		r2.x * p0.z + r2.y * p1.z + r2.z * p2.z + r2.w * p3.z
		);
}

glm::vec3 Spline::pointDirection(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t)
{
	glm::vec4 r1 = (1.0f / 2.0f) * glm::vec4(3 * t*t, 2 * t, 1, 0);
	glm::vec4 r2 = r1 * glm::mat4(
		glm::vec4(-1, 2, -1, 0),
		glm::vec4(3, -5, 0, 2),
		glm::vec4(-3, 4, 1, 0),
		glm::vec4(1, -1, 0, 0)
		);
	return glm::vec3(
		r2.x * p0.x + r2.y * p1.x + r2.z * p2.x + r2.w * p3.x,
		r2.x * p0.y + r2.y * p1.y + r2.z * p2.y + r2.w * p3.y,
		r2.x * p0.z + r2.y * p1.z + r2.z * p2.z + r2.w * p3.z
		);
}
