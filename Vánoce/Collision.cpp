#include "headers\Collision.h"

Collision::Collision(Object * object)
{
	vector<float> x_values;
	vector<float> y_values;
	vector<float> z_values;

	int nAttrPerVertex = object->model->nAttrPerVert;
	int nVert = object->model->nVert;

	for (int i = 0; i < nAttrPerVertex * nVert; i += nAttrPerVertex)
		x_values.push_back(object->model->vertices[i]);
	for (int i = 1; i < nAttrPerVertex * nVert; i += nAttrPerVertex)
		y_values.push_back(object->model->vertices[i]);
	for (int i = 2; i < nAttrPerVertex * nVert; i += nAttrPerVertex)
		z_values.push_back(object->model->vertices[i]);

	float x_min = *min_element(x_values.begin(), x_values.end());
	float x_max = *max_element(x_values.begin(), x_values.end());
	float y_min = *min_element(y_values.begin(), y_values.end());
	float y_max = *max_element(y_values.begin(), y_values.end());
	float z_min = *min_element(z_values.begin(), z_values.end());
	float z_max = *max_element(z_values.begin(), z_values.end());

	this->homeBoundariesMin = glm::vec3(x_min, y_min, z_min);
	this->homeBoundariesMax = glm::vec3(x_max, y_max, z_max);
}

Collision::Collision(int nAttrPerVertex, int nVert, const float * vertices)
{
	list<float> y_values;
	for (int i = 1; i < nAttrPerVertex * nVert; i += nAttrPerVertex)
		y_values.push_back(vertices[i]);

	this->ornamentDiameter = *max_element(y_values.begin(), y_values.end()) - *min_element(y_values.begin(), y_values.end());
}

Collision::~Collision()
{
}

bool Collision::check(glm::vec3 world_position)
{
	if (world_position.x >= homeBoundariesMin.x && world_position.x <= homeBoundariesMax.x &&
		world_position.y >= homeBoundariesMin.y && world_position.y <= homeBoundariesMax.y &&
		world_position.z >= homeBoundariesMin.z && world_position.z <= homeBoundariesMax.z)
		return true;
	else
		return false;
}

bool Collision::sphereCheck(glm::vec3 position)
{
	for (list<glm::vec3>::iterator i = ornamentPositions.begin(); i != ornamentPositions.end(); i++)
		if (glm::length((*i) - position) < 5 * ornamentDiameter) return true;
	return false;
}

void Collision::addOrnamentPosition(glm::vec3 position)
{
	ornamentPositions.push_back(position);
}
