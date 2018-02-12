#pragma once

#include "XMLParser.h"
#include "Database\Database.h"

#include <string>
#include <vector>

class Level
{
public:
	Level(Database* database);
	~Level();

	void loadLevelFile(std::string levelFilePath);
	void unloadLevel();

private:
	XMLParser parser;
	std::vector<std::string> listOfObjectTypesInLevel;
	Database *database;
};

