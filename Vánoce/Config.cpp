#include "headers\Config.h"

Config::Config()
{
	load();
}

Config::~Config()
{
}

void Config::load()
{
	ifstream configFile("config/config.txt");

	string line;
	while (getline(configFile, line))
	{
		if (line[0] == '#' || line.empty())	continue;

		size_t valueKeyDelimiter = line.find("=");
		string key = line.substr(0, valueKeyDelimiter);
		string value = line.substr(valueKeyDelimiter + 1, string::npos);

		size_t vec3Delimiter = value.find(",");
		size_t floatDelimiter = value.find(".");

		if (vec3Delimiter != string::npos) // glm::vec3
		{
			float x = stof(value.substr(0, vec3Delimiter));
			value.erase(0, vec3Delimiter + 1);

			vec3Delimiter = value.find(",");
			float y = stof(value.substr(0, vec3Delimiter));
			value.erase(0, vec3Delimiter + 1);

			vec3Delimiter = value.find(",");
			float z = stof(value.substr(0, vec3Delimiter));
			value.erase(0, vec3Delimiter + 1);

			v3Options[key] = glm::vec3(x, y, z);
		}
		else if (vec3Delimiter == string::npos && floatDelimiter != string::npos) // float
			fOptions[key] = stof(value);
		else // int
			fOptions[key] = (float)(stoi(value));
	}
}

glm::vec3 Config::vOpt(string key)
{
	return v3Options[key];
}

float Config::fOpt(string key)
{
	return fOptions[key];
}
