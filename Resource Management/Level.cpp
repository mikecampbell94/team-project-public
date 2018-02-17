#include "Level.h"

#include <fstream>
#include <stdexcept>

#include "../Graphics/Utility/Light.h"

Level::Level(Database *database,SceneManager* sceneManager)
{
	this->database = database;
	this->sceneManager = sceneManager;
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

	addObjectsToGame();
}

void Level::unloadLevel()
{
	for (std::vector<std::string>::const_iterator it = listOfObjectTypesInLevel.begin(); it != listOfObjectTypesInLevel.end(); ++it)
	{
		database->getTable(*it)->getAllResources()->deleteAllResources();
	}
}

void Level::addObjectsToGame()
{
	auto gameObjectResources = database->getTable("GameObjects")->getAllResources()->getResourceBuffer();
	for (auto gameObjectIterator = gameObjectResources.begin(); gameObjectIterator != gameObjectResources.end(); gameObjectIterator++)
	{
		(*sceneManager->getAllNodes())->push_back(static_cast<GameObject*>((*gameObjectIterator).second)->getSceneNode());
	}

	auto lightsResources = database->getTable("Lights")->getAllResources()->getResourceBuffer();
	for (auto lightsIterator = lightsResources.begin(); lightsIterator != lightsResources.end(); lightsIterator++)
	{
		(*sceneManager->getAllLights())->push_back(static_cast<Light*>((*lightsIterator).second));
	}
}