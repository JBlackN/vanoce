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

void Hud::draw(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	if (!this->enabled) return;

	this->hudOrnamentRed->draw(left, right, bottom, top, nearPlane, farPlane);
	this->hudOrnamentYellow->draw(left, right, bottom, top, nearPlane, farPlane);
	this->hudOrnamentBlue->draw(left, right, bottom, top, nearPlane, farPlane);
}
