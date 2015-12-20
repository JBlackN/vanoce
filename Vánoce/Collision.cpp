#include "headers\Collision.h"

Collision::Collision(Object * object)
{
	vector<float> x_values;
	vector<float> y_values;
	vector<float> z_values;

	int nAttrPerVertex = object->model->nAttrPerVert;
	int nVert = object->model->nVert;

	for (int i = 0; i < nAttrPerVertex * nVert; i += object->model->nAttrPerVert)
		x_values.push_back(object->model->vertices[i]);
	for (int i = 1; i < nAttrPerVertex * nVert; i += object->model->nAttrPerVert)
		y_values.push_back(object->model->vertices[i]);
	for (int i = 2; i < nAttrPerVertex * nVert; i += object->model->nAttrPerVert)
		z_values.push_back(object->model->vertices[i]);

	float x_min = *min_element(x_values.begin(), x_values.end());
	float x_max = *max_element(x_values.begin(), x_values.end());
	float y_min = *min_element(y_values.begin(), y_values.end());
	float y_max = *max_element(y_values.begin(), y_values.end());
	float z_min = *min_element(z_values.begin(), z_values.end());
	float z_max = *max_element(z_values.begin(), z_values.end());

	this->boundaries_min = glm::vec3(x_min, y_min, z_min);
	this->boundaries_max = glm::vec3(x_max, y_max, z_max);
}

Collision::~Collision()
{
}

bool Collision::check(glm::vec3 world_position)
{
	if (world_position.x >= boundaries_min.x && world_position.x <= boundaries_max.x &&
		world_position.y >= boundaries_min.y && world_position.y <= boundaries_max.y &&
		world_position.z >= boundaries_min.z && world_position.z <= boundaries_max.z)
		return true;
	else
		return false;
}
