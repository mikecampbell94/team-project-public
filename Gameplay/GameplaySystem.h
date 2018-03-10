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

	void updateSubsystem(const float& deltaTime) override;

	void connectPlayerbase(PlayerBase* playerbase);
	void compileGameplayScript(std::string levelScript);
	void setDefaultGameplayScript();

	void addGameObjectScript(std::string scriptFile);
	void deleteGameObjectScripts();
	void compileGameObjectScripts();

	//hardcoded test stuff REMOVE!
	void setTimedLevel(float maxTime)
	{
		gameLogic.maxTime = maxTime;
		gameLogic.isTimed = true;
	}
	void setUnTimedLevel()
	{
		gameLogic.isTimed = false;
	}

	std::string getGameplayFile()
	{
		return gameLogic.getScriptFile();
	}

	std::vector<GameObjectLogic>* getGameObjectLogics()
	{
		return &objects;
	}

private:
	GameLogic gameLogic;
	std::vector<GameObjectLogic> objects;
	GameplayInputBridge inputBridge;
	XMLParser inputParser;
	Database* database;

	std::vector<std::string> gameObjectLogicRemoveBuffer;
	bool levelFinished = false;
};

