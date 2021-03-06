#include "headers\Inventory.h"

Inventory::Inventory(Config * config, Model * redOrnament, Model * yellowOrnament, Model * blueOrnament)
{
	this->config = config;

	this->redOrnament = redOrnament;
	this->yellowOrnament = yellowOrnament;
	this->blueOrnament = blueOrnament;

	this->nRedOrnaments = 0;
	this->nYellowOrnaments = 0;
	this->nBlueOrnaments = 0;

	this->collision = new Collision(config, redOrnament->nAttrPerVert, redOrnament->nVert, redOrnament->vertices);
}

Inventory::~Inventory()
{
	delete collision;
	placedOrnaments[red].clear();
	placedOrnaments[yellow].clear();
	placedOrnaments[blue].clear();
}

void Inventory::insertOrnament(OrnamentType type, unsigned int count)
{
	switch (type)
	{
	case red:
		this->nRedOrnaments += count;
		break;
	case yellow:
		this->nYellowOrnaments += count;
		break;
	case blue:
		this->nBlueOrnaments += count;
		break;
	}
}

void Inventory::removeOrnament(OrnamentType type)
{
	switch (type)
	{
	case red:
		if (this->nRedOrnaments > 0) this->nRedOrnaments--;
		break;
	case yellow:
		if (this->nYellowOrnaments > 0) this->nYellowOrnaments--;
		break;
	case blue:
		if (this->nBlueOrnaments > 0) this->nBlueOrnaments--;
		break;
	}
}

unsigned int Inventory::ornamentCount(OrnamentType type)
{
	switch (type)
	{
	case red:
		return this->nRedOrnaments;
	case yellow:
		return this->nYellowOrnaments;
	case blue:
		return this->nBlueOrnaments;
	}
}

void Inventory::placeOrnament(OrnamentType type, Object * tree, glm::vec2 window_dimensions,
	int windowX, int windowY, Camera * camera)
{
	if (ornamentCount(type) == 0) return;

	Model * ornamentModel;
	switch (type)
	{
	default:
	case red:
		ornamentModel = this->redOrnament;
		break;
	case yellow:
		ornamentModel = this->yellowOrnament;
		break;
	case blue:
		ornamentModel = this->blueOrnament;
		break;
	}

	glm::vec3 position = findPosition(tree, window_dimensions, windowX, windowY, camera);

	if (collision->sphereCheck(position)) return;
	collision->addOrnamentPosition(position);

	Object * placedOrnament = new Object(ornamentModel, glm::translate(glm::scale(glm::mat4(),
		config->fOpt("scale") * glm::vec3(5, 5, 5)), position / (config->fOpt("scale") * 5.0f)));
	this->placedOrnaments[type].push_back(placedOrnament);

	removeOrnament(type);
}

void Inventory::drawOrnaments(Camera * camera, map<string, Light*> lights, Fog * fog)
{
	if (placedOrnaments.empty()) return;

	// Red ornaments
	if (placedOrnaments.find(red) != placedOrnaments.end() && !placedOrnaments[red].empty())
	{
		(*(placedOrnaments[red].begin()))->model->material->emission = glm::vec4(0.9f, 0, 0.2f, 1);

		for (list<Object *>::iterator r = placedOrnaments[red].begin(); r != placedOrnaments[red].end(); r++)
			(*r)->draw(camera, lights, fog);

		(*(placedOrnaments[red].begin()))->model->material->emission = glm::vec4(0, 0, 0, 1);
	}

	// Yellow ornaments
	if (placedOrnaments.find(yellow) != placedOrnaments.end() && !placedOrnaments[yellow].empty())
	{
		(*(placedOrnaments[yellow].begin()))->model->material->emission = glm::vec4(1, 0.9f, 0, 1);

		for (list<Object *>::iterator y = placedOrnaments[yellow].begin(); y != placedOrnaments[yellow].end(); y++)
			(*y)->draw(camera, lights, fog);

		(*(placedOrnaments[yellow].begin()))->model->material->emission = glm::vec4(0, 0, 0, 1);
	}

	// Blue ornaments
	if (placedOrnaments.find(blue) != placedOrnaments.end() && !placedOrnaments[blue].empty())
	{
		(*(placedOrnaments[blue].begin()))->model->material->emission = glm::vec4(0, 0.8f, 1, 1);

		for (list<Object *>::iterator b = placedOrnaments[blue].begin(); b != placedOrnaments[blue].end(); b++)
			(*b)->draw(camera, lights, fog);

		(*(placedOrnaments[blue].begin()))->model->material->emission = glm::vec4(0, 0, 0, 1);
	}
}

glm::vec3 Inventory::findPosition(Object * tree, glm::vec2 window_dimensions, int windowX, int windowY, Camera * camera)
{
	glm::vec3 ray_start, ray_end;

	GLint winWidth = glutGet(GLUT_WINDOW_WIDTH);
	GLint winHeight = glutGet(GLUT_WINDOW_HEIGHT);

	glm::vec4 viewport = glm::vec4(0, 0, winWidth, winHeight);
	GLfloat winZ;
	glReadPixels((GLint)windowX, (GLint)(winHeight - windowY - 1), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	glm::vec3 winCoords = glm::vec3(windowX, winHeight - windowY - 1, winZ);

	return glm::unProject(winCoords, camera->getViewMatrix(), camera->getProjectionMatrix(), viewport);
}
