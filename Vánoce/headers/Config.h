#pragma once

#include <iostream> // TODO: REMOVE
#include <fstream>
#include <string>
#include <map>
using namespace std;

#include "pgr.h"

class Config
{
private:
	map<string, glm::vec3> v3Options;
	map<string, float> fOptions;
public:
	Config();
	~Config();

	void load();

	glm::vec3 vOpt(string key);
	float fOpt(string key);
};

