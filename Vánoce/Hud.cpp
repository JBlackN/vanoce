#include "headers\Hud.h"

Hud::Hud(int winWidth, int winHeight, Shader * shader, map<string, Material *> materials, map<string, Texture *> textures,
	bool enabled)
{
	this->enabled = enabled;

	this->hudElementVertices = new float[4*8] {
		winWidth - (hudElementSizePx + (winWidth / 100)*hudElementBorderPercent), winHeight - (winHeight / 100)*hudElementBorderPercent, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		winWidth - (hudElementSizePx + (winWidth / 100)*hudElementBorderPercent), winHeight - (hudElementSizePx + (winHeight / 100)*hudElementBorderPercent), 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		winWidth - (winWidth / 100)*hudElementBorderPercent, winHeight - (winHeight / 100)*hudElementBorderPercent, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		winWidth - (winWidth / 100)*hudElementBorderPercent, winHeight - (hudElementSizePx + (winHeight / 100)*hudElementBorderPercent), 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f
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
	if (!this->enabled) return;

	Texture * redNumberTexture;
	Texture * yellowNumberTexture;
	Texture * blueNumberTexture;

	unsigned int redOrnamentCount = inventory->ornamentCount(Inventory::OrnamentType::red);
	if (redOrnamentCount > 0 && redOrnamentCount < 10)
		redNumberTexture = numbers[redOrnamentCount - 1];
	else if (redOrnamentCount > 9)
		redNumberTexture = numbers[8];
	else
		redNumberTexture = NULL;

	unsigned int yellowOrnamentCount = inventory->ornamentCount(Inventory::OrnamentType::yellow);
	if (yellowOrnamentCount > 0 && yellowOrnamentCount < 10)
		yellowNumberTexture = numbers[yellowOrnamentCount - 1];
	else if (yellowOrnamentCount > 9)
		yellowNumberTexture = numbers[8];
	else
		yellowNumberTexture = NULL;

	unsigned int blueOrnamentCount = inventory->ornamentCount(Inventory::OrnamentType::blue);
	if (blueOrnamentCount > 0 && blueOrnamentCount < 10)
		blueNumberTexture = numbers[blueOrnamentCount - 1];
	else if (blueOrnamentCount > 9)
		blueNumberTexture = numbers[8];
	else
		blueNumberTexture = NULL;

	this->hudOrnamentRed->draw(redNumberTexture, left, right, bottom, top, nearPlane, farPlane);
	this->hudOrnamentYellow->draw(yellowNumberTexture, left, right, bottom, top, nearPlane, farPlane);
	this->hudOrnamentBlue->draw(blueNumberTexture, left, right, bottom, top, nearPlane, farPlane);
}
