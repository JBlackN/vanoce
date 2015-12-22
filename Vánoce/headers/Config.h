#pragma once

#include <fstream>
#include <string>
#include <map>
using namespace std;

#include "pgr.h"

/**
 * Class loads configuration values from a file and provides access to them.
 */
class Config
{
private:
	map<string, glm::vec3> v3Options; ///< Glm::vec3 options
	map<string, float> fOptions; ///< Float options
public:
	Config();
	~Config();

	void load();

	/**
	 * Glm::vec3 option getter.
	 * @param key Key the option is stored under.
	 * @return Glm::vec3 option value.
	 */
	glm::vec3 vOpt(string key);

	/**
	* Float option getter.
	* @param key Key the option is stored under.
	* @return Float option value.
	*/
	float fOpt(string key);
};

