//----------------------------------------------------------------------------------------
/**
* \file       Spline.h
* \author     Petr Schmied
* \date       2016/01/05
* \brief      File contains Spline class.
*
*  File contains Spline class, its variables and methods.
*
*/
//----------------------------------------------------------------------------------------
#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

#include "pgr.h"
#include "Config.h"

/**
* Class describes Catmull-Rom spline.
*/
class Spline
{
public:
	/**
	* Class describes one point on Catmull-Rom spline.
	*/
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
	Config * config; ///< Config provider.
	vector<Point *> points;
public:
	/**
	* Generates random Catmull-Rom spline.
	* @param config Config provider.
	* @param start_position Spline's first point.
	* @param pointCount Number of spline's control points.
	* @param pointDirectionAngle Limits spline's 'width' (NOT USED IN CURRENT VERSION).
	*/
	Spline(Config * config, glm::vec3 start_position, int pointCount, float pointDirectionAngle = 90.0f);

	/**
	* Generates Catmull-Rom spline specified by provided control points.
	* @param config Config provider.
	* @param points Control points.
	*/
	Spline(Config * config, vector<Point *> points);

	~Spline();

	/**
	* Gets point on Catmull-Rom spline based on specified animation frame and total frame count.
	* @param frame Current animation frame.
	* @param frameCount Total frame count.
	* @return Specific point on Catmull-Rom spline.
	*/
	Point * point(int frame, int frameCount);
private:
	/**
	* Gets position coordinates of a point which lies on Catmull-Rom spline's segment (p1-p2).
	* @param p0 First control point.
	* @param p1 Second control point (start of segment).
	* @param p2 Third control point (end of segment).
	* @param p3 Fourth control point.
	* @param t Point's position on spline's segment.
	* @return Point's position coordinates.
	*/
	glm::vec3 pointPosition(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t);

	/**
	* Gets direction coordinates of a point which lies on Catmull-Rom spline's segment (p1-p2).
	* @param p0 First control point.
	* @param p1 Second control point (start of segment).
	* @param p2 Third control point (end of segment).
	* @param p3 Fourth control point.
	* @param t Point's position on spline's segment.
	* @return Point's direction coordinates.
	*/
	glm::vec3 pointDirection(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, float t);
};

