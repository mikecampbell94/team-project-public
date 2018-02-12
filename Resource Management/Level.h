#pragma once

#include "XMLParser.h"
#include "Database\Database.h"
#include "Database\TableCreation.h"


#include <string>
#include <vector>

#define DATADIR "../Data/"
#define LEVELDIR "../Data/Levels/"


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

