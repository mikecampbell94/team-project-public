#include "Level.h"

#include <fstream>
#include <stdexcept>

#include "../Graphics/Utility/Light.h"
#include "../Interface/UserInterface.h"
#include "../Gameplay/GameplaySystem.h"

Level::Level(Database *database, SceneManager* sceneManager,
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
	//std::string lvlStr = LEVELDIR;
	parser.loadFile(levelFilePath);
	levelNode = *parser.parsedXml;
	for (Node* child : levelNode.children)
	{
		if (child->nodeType == "UI")
		{
			userInterface->initialise(child->children[0]->value, database);
		}
		else if (child->nodeType == "GamePlay")
		{
			gameplay->compileGameplayScript(child->children[0]->value);
		}
		else if (child->nodeType == "GameLogic")
		{
			for (Node* grandChild : child->children)
			{
				gameplay->addGameObjectScript(grandChild->value);
			}
		}
		else
		{
			parser.loadFile(LEVELDIR + child->value);
			for (Node* grandchild : parser.parsedXml->children)
			{
				listOfObjectTypesInLevel.insert(grandchild->nodeType);

				database->addResourceToTable(grandchild->nodeType, grandchild);
			}

		}

	}



	addObjectsToGame();
}

void Level::unloadLevelWhileKeepingUserInterface()
{
	//send message to renderer to clear score ui

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
	//send message to renderer to clear score ui

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
		if (pnode != nullptr)
			physics->addPhysicsObject(pnode);
	}

	auto lightsResources = database->getTable("Lights")->getAllResources()->getResourceBuffer();
	for (auto lightsIterator = lightsResources.begin(); lightsIterator != lightsResources.end(); lightsIterator++)
	{
		(*sceneManager->getAllLights())->push_back(static_cast<Light*>((*lightsIterator).second));
	}
}