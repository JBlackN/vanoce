#include "headers\Hud.h"

Hud::Hud(int winWidth, int winHeight, Shader * shader, map<string, Material *> materials, map<string, Texture *> textures,
	float hudElementSizePx, float hudElementBorderPercent, bool enabled)
{
	this->enabled = enabled;
	this->hudElementSizePx = hudElementSizePx;
	this->hudElementBorderPercent = hudElementBorderPercent;

	this->hudElementVertices = new float[4*8] {
		winWidth - (hudElementSizePx + (winWidth / 100)*hudElementBorderPercent), 
		winHeight - (winHeight / 100)*hudElementBorderPercent, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		winWidth - (hudElementSizePx + (winWidth / 100)*hudElementBorderPercent),
		winHeight - (hudElementSizePx + (winHeight / 100)*hudElementBorderPercent), 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

		winWidth - (winWidth / 100)*hudElementBorderPercent,
		winHeight - (winHeight / 100)*hudElementBorderPercent, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,

		winWidth - (winWidth / 100)*hudElementBorderPercent,
		winHeight - (hudElementSizePx + (winHeight / 100)*hudElementBorderPercent), 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f
	};

	this->hudElementTriangles = new unsigned int[2*3] {
		0, 1, 2,
		1, 2, 3
	};

	Model * ornamentRedModel = new Model(shader, materials["ornament_red_hud"], textures["ornament_red_hud"],
		8, 4, 2, hudElementVertices, hudElementTriangles);
	Model * ornamentYellowModel = new Model(shader, materials["ornament_yellow_hud"], textures["ornament_yellow_hud"],
		8, 4, 2, hudElementVertices, hudElementTriangles);
	Model * ornamentBlueModel = new Model(shader, materials["ornament_blue_hud"], textures["ornament_blue_hud"],
		8, 4, 2, hudElementVertices, hudElementTriangles);

	this->hudOrnamentRed = new Object(ornamentRedModel, glm::translate(glm::mat4(1), glm::vec3(-2 * (48), 0, 0)));;
	this->hudOrnamentYellow = new Object(ornamentYellowModel, glm::translate(glm::mat4(1), glm::vec3(-1 * (48), 0, 0)));;
	this->hudOrnamentBlue = new Object(ornamentBlueModel, glm::translate(glm::mat4(1), glm::vec3(-0 * (48), 0, 0)));;

	this->numbers.push_back(textures["ornament_1_hud"]);
	this->numbers.push_back(textures["ornament_2_hud"]);
	this->numbers.push_back(textures["ornament_3_hud"]);
	this->numbers.push_back(textures["ornament_4_hud"]);
	this->numbers.push_back(textures["ornament_5_hud"]);
	this->numbers.push_back(textures["ornament_6_hud"]);
	this->numbers.push_back(textures["ornament_7_hud"]);
	this->numbers.push_back(textures["ornament_8_hud"]);
	this->numbers.push_back(textures["ornament_9_hud"]);
}

Hud::~Hud()
{
}

void Hud::draw(Inventory * inventory, float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	if (!enabled) return;

	Texture * redNumberTexture = getNumberTexture(inventory, Inventory::OrnamentType::red);
	Texture * yellowNumberTexture = getNumberTexture(inventory, Inventory::OrnamentType::yellow);
	Texture * blueNumberTexture = getNumberTexture(inventory, Inventory::OrnamentType::blue);

	if (redNumberTexture != NULL) hudOrnamentRed->model->textures.push_back(redNumberTexture);
	hudOrnamentRed->draw(left, right, bottom, top, nearPlane, farPlane);
	if (redNumberTexture != NULL) hudOrnamentRed->model->textures.pop_back();

	if (yellowNumberTexture != NULL) hudOrnamentYellow->model->textures.push_back(yellowNumberTexture);
	hudOrnamentYellow->draw(left, right, bottom, top, nearPlane, farPlane);
	if (yellowNumberTexture != NULL) hudOrnamentYellow->model->textures.pop_back();

	if (blueNumberTexture != NULL) hudOrnamentBlue->model->textures.push_back(blueNumberTexture);
	hudOrnamentBlue->draw(left, right, bottom, top, nearPlane, farPlane);
	if (blueNumberTexture != NULL) hudOrnamentBlue->model->textures.pop_back();
}

Texture * Hud::getNumberTexture(Inventory * inventory, Inventory::OrnamentType type)
{
	unsigned int redOrnamentCount = inventory->ornamentCount(Inventory::OrnamentType::red);
	unsigned int yellowOrnamentCount = inventory->ornamentCount(Inventory::OrnamentType::yellow);
	unsigned int blueOrnamentCount = inventory->ornamentCount(Inventory::OrnamentType::blue);

	switch (type)
	{
	case Inventory::OrnamentType::red:
		if (redOrnamentCount > 0 && redOrnamentCount < 10)
			return numbers[redOrnamentCount - 1];
		else if (redOrnamentCount > 9)
			return numbers[8];
		else
			return NULL;

	case Inventory::OrnamentType::yellow:
		if (yellowOrnamentCount > 0 && yellowOrnamentCount < 10)
			return numbers[yellowOrnamentCount - 1];
		else if (yellowOrnamentCount > 9)
			return numbers[8];
		else
			return NULL;

	case Inventory::OrnamentType::blue:
		if (blueOrnamentCount > 0 && blueOrnamentCount < 10)
			return numbers[blueOrnamentCount - 1];
		else if (blueOrnamentCount > 9)
			return numbers[8];
		else
			return NULL;

	default:
		return NULL;
	}
}
