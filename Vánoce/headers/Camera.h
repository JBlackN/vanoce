#pragma once

#include <cstdlib>
#include <vector>
using namespace std;

#include "pgr.h"
#include"Config.h"
#include "Spline.h"

/**
 * Class describes camera with perspective projection. Movement, looking and animation are supported.
 */
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

	bool moving; ///< Can camera move?
	bool looking; ///< Can camera look?
	bool animated;

	Spline * path;
	int frame; ///< Current frame of (animated) camera animation.
	int seconds; ///< Duration of (animated) camera animation.

	enum Direction { forward, back, left, right };
private:
	Config * config; ///< Config provider.
public:
	Camera(Config * config,
		float fov, float aspectRatio, float nearPlane, float farPlane,
		glm::vec3 position, glm::vec3 center, glm::vec3 up,
		bool moving = false, bool looking = false, bool animated = false);
	~Camera();

	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	void move(Direction whereTo);

	/**
	 * Handles camera looking (both horizontal and vertical).
	 * @param window_x Cursor position to look at (x-coordinate).
	 * @param window_y Cursor position to look at (y-coordinate).
	 * @param cursor_position Previous cursor position (used to determine direction).
	 * @param window_dimensions Window dimensions.
	 */
	void look(int window_x, int window_y, glm::vec2 & cursor_position, glm::vec2 window_dimensions);

	/**
	 * Updates animated camera's position and direction based on current frame.
	 */
	void update();
private:
	/**
	 * Checks if camera stays within the scene. Parameters are used to revert movement leading out of scene.
	 * @param direction Direction of the last movement (forwards of backwards).
	 * @param amount Amount of the last movement.
	 */
	void checkBoundaries(Direction direction, glm::vec3 amount);
};

