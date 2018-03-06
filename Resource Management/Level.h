#pragma once

#include "XMLParser.h"
#include "Database\Database.h"
#include "Database\TableCreation.h"
#include "../Graphics/Scene Management/SceneManager.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsEngine.h"
#include <string>
#include <vector>
#include <set>
#include "../Utilities/FilePaths.h"


class UserInterface;
class GameplaySystem;

class Level
{
public:
	Level(Database* database, SceneManager* sceneManager,
		PhysicsEngine* physics, UserInterface* userInterface);
	~Level();

	void loadLevelFile(std::string levelFilePath, GameplaySystem* gameplay);

	void unloadLevelWhileKeepingUserInterface();
	void unloadLevel();

	void addObjectsToGame();


private:
	XMLParser parser;
	std::set<std::string> listOfObjectTypesInLevel;
	Database *database;
	SceneManager* sceneManager;
	PhysicsEngine* physics;
	UserInterface* userInterface;
	Node levelNode;
};