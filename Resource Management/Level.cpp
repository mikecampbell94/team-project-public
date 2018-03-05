#include "Level.h"

#include <fstream>
#include <stdexcept>

#include "../Graphics/Utility/Light.h"
#include "../Interface/UserInterface.h"
#include "../Gameplay/GameplaySystem.h"

Level::Level(Database *database,SceneManager* sceneManager, 
	PhysicsEngine* physics, UserInterface* userInterface)
{
	this->database = database;
	this->sceneManager = sceneManager;
	this->physics = physics;
	this->userInterface = userInterface;
}

Level::~Level()
{
}

void Level::loadLevelFile(std::string levelFilePath, GameplaySystem* gameplay)
{
	gameplay->deleteGameObjectScripts();

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
		if (line.find("UserInterface/") != string::npos)
		{
			userInterface->initialise(line, database);
		}
		else if (line.find("Gameplay/") != string::npos)
		{
			gameplay->compileGameplayScript(line);
		}
		else if (line.find("GameObjectLogic/") != string::npos)
		{
			gameplay->addGameObjectScript(line);
		}
		else
		{
			//listOfObjectTypesInLevel.push_back(parser.loadFile(DATADIR + line));
			parser.loadFile(DATADIR + line);
			//have our object in XML format, add to database :)
			for (Node * child : parser.parsedXml->children)
			{
				listOfObjectTypesInLevel.insert(child->nodeType);
				database->addResourceToTable(child->nodeType, child);
			}
		}
	}
	levelFile.close();

	addObjectsToGame();
}

void Level::unloadLevelWhileKeepingUserInterface()
{
	MoveCameraRelativeToGameObjectMessage::resourceName = "";
	(*sceneManager->getAllNodes())->clear();
	(*sceneManager->getAllLights())->clear();
	physics->removeAllPhysicsObjects();

	for each (std::string table in listOfObjectTypesInLevel)
	{
		if (table != "UIMeshes")
		{
			database->getTable(table)->getAllResources()->deleteAllResources();
		}
	}
}

void Level::unloadLevel()
{
	MoveCameraRelativeToGameObjectMessage::resourceName = "";
	(*sceneManager->getAllNodes())->clear();
	(*sceneManager->getAllLights())->clear();
	physics->removeAllPhysicsObjects();

	//for (std::vector<std::string>::const_iterator it = listOfObjectTypesInLevel.begin(); it != listOfObjectTypesInLevel.end(); ++it)
	for each (std::string table in listOfObjectTypesInLevel)
	{
		database->getTable(table)->getAllResources()->deleteAllResources();
	}
}

void Level::addObjectsToGame()
{
	auto gameObjectResources = database->getTable("GameObjects")->getAllResources()->getResourceBuffer();
	for (auto gameObjectIterator = gameObjectResources.begin(); gameObjectIterator != gameObjectResources.end(); gameObjectIterator++)
	{
		(*sceneManager->getAllNodes())->push_back(static_cast<GameObject*>((*gameObjectIterator).second)->getSceneNode());
		PhysicsNode* pnode = static_cast<GameObject*>((*gameObjectIterator).second)->getPhysicsNode();
		if(pnode != nullptr)
			physics->addPhysicsObject(pnode);
	}

	auto lightsResources = database->getTable("Lights")->getAllResources()->getResourceBuffer();
	for (auto lightsIterator = lightsResources.begin(); lightsIterator != lightsResources.end(); lightsIterator++)
	{
		(*sceneManager->getAllLights())->push_back(static_cast<Light*>((*lightsIterator).second));
	}
}