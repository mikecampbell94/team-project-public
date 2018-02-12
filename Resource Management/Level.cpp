#include "Level.h"

#include <fstream>
#include <stdexcept>

Level::Level(Database *database)
{
	this->database = database;
}

Level::~Level()
{
}

void Level::loadLevelFile(std::string levelFilePath)
{
	std::ifstream levelFile;

	levelFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);


	levelFile.open(LEVELDIR + levelFilePath);

	if (levelFile.fail())
	{
		throw std::runtime_error("Error: File " +  levelFilePath + " not found");
	}

	std::string line;
	
	while (!levelFile.eof() && std::getline(levelFile, line))
	{
		listOfObjectTypesInLevel.push_back(parser.loadFile(DATADIR + line));
		//have our object in XML format, add to database :)
		for (Node * child : parser.parsedXml->children) {
			database->addResourceToTable(child->nodeType, child);
		}
		
	}
	levelFile.close();
}

void Level::unloadLevel()
{
	for (std::vector<std::string>::const_iterator it = listOfObjectTypesInLevel.begin(); it != listOfObjectTypesInLevel.end(); ++it)
	{
		database->getTable(*it)->getAllResources()->deleteAllResources();
	}
}