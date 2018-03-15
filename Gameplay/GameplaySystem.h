#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "InputGameplay/GameplayInputBridge.h"
#include "Scripting/GameLogic.h"
#include "../Resource Management/XMLParser.h"
#include "GameObjectLogic/GameObjectLogic.h"
#include "../Resource Management/Database/Database.h"

class Database;

class GameplaySystem : public Subsystem
{
public:
	GameplaySystem(Database* database);
	~GameplaySystem();

	void updateNextFrame(const float& deltaTime) override;

	void connectPlayerbase(PlayerBase* playerbase);
	void compileGameplayScript(std::string levelScript);
	void setDefaultGameplayScript();

	void addGameObjectScript(std::string scriptFile);
	void deleteGameObjectScripts();
	void compileGameObjectScripts();

	std::string getGameplayFile()
	{
		return gameplayScript;
	}

	std::vector<GameObjectLogic>* getGameObjectLogics()
	{
		return &objects;
	}

private:
	void updateGameLogic(const float& deltaTime);

	GameLogic gameLogic;
	std::vector<GameObjectLogic> objects;
	GameplayInputBridge inputBridge;
	XMLParser inputParser;
	Database* database;

	std::vector<std::string> gameObjectLogicRemoveBuffer;
	std::string gameplayScript = "";
	bool levelFinished = false;

	std::map<int, int> playerScores;
};

