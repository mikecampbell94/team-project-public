#pragma once

#include "XMLParser.h"
#include "Database\Database.h"
#include "Database\TableCreation.h"
#include "../Graphics/Scene Management/SceneManager.h"
#include "../Gameplay/GameObject.h"
#include <string>
#include <vector>
#include <set>

#define DATADIR "../Data/"
#define LEVELDIR "../Data/Levels/"


class Level
{
public:
	Level(Database* database, SceneManager* sceneManager);
	~Level();

	void loadLevelFile(std::string levelFilePath);

	void unloadLevelWhileKeepingUserInterface();
	void unloadLevel();

	void addObjectsToGame();


private:
	XMLParser parser;
	std::set<std::string> listOfObjectTypesInLevel;
	Database *database;
	SceneManager* sceneManager;
};

