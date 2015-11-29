#pragma once

#include "pgr.h"

class Camera
{
public:
	Camera();
	Camera(float, float, float, float, glm::vec3, glm::vec3, glm::vec3);
	~Camera();
private:
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 up;
public:
	glm::mat4 getCameraMatrix();
	float getFov();
	void setFov(float);
	float getAspectRatio();
	void setAspectRatio(float);
	float getNearPlane();
	void setNearPlane(float);
	float getFarPlane();
	void setFarPlane(float);
	glm::vec3 getPosition();
	void setPosition(glm::vec3);
	glm::vec3 getDirection();
	void setDirection(glm::vec3);
	glm::vec3 getUp();
	void setUp(glm::vec3);
};

