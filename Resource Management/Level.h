#pragma once

#include "XMLParser.h"
#include "Database\Database.h"
#include "Database\TableCreation.h"
#include "../Graphics/Scene Management/SceneManager.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsEngine.h"
#include <string>
#include <vector>

#define DATADIR "../Data/"
#define LEVELDIR "../Data/Levels/"


class Level
{
public:
	Level(Database* database, SceneManager* sceneManager, PhysicsEngine* physics);
	~Level();

	void loadLevelFile(std::string levelFilePath);
	void unloadLevel();

	void addObjectsToGame();


private:
	XMLParser parser;
	std::vector<std::string> listOfObjectTypesInLevel;
	Database *database;
	SceneManager* sceneManager;
	PhysicsEngine* physics;
};

