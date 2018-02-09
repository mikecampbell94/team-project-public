#pragma once

#include "XMLParser.h"

#include <string>
#include <vector>

class Level
{
public:
	Level();
	~Level();

	void loadLevelFile(std::string levelFilePath);
	void unloadLevel();

private:
	XMLParser parser;
	std::vector<std::string> listOfObjectTypesInLevel;
};

