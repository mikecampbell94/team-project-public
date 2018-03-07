#pragma once

#include "../Communication/Message.h"
#include "../Gameplay/GameObject.h"
#include "../Gameplay/Scripting/ActionBuilder.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/Scripting/GameLogic.h"
#include "../../Communication/Messages/PlayerInputMessage.h"

class Database;

class GameObjectLogic
{
public:
	GameObjectLogic(Database* database, MessageProcessor* messages, std::string script);
	~GameObjectLogic();

	void compileParsedXMLIntoScript();
	void notify(const std::string& messageType, Message* message);
	void updatelogic(const float& deltaTime);
	
	std::string getScriptFile()
	{
		return scriptFile;
	}

private:
	void compileFunctions(Node* node);
	void compileFunctionsOnStart(Node* node);
	void updateHardCodedLogic(const float& deltaTime);
	void updateInputMessageLogic();

	Node* parsedScript;
	std::map<GameObject*, GameLogic> logicToGameObjects;
	std::vector<GameLogic*> logics;
	Database* database;
	MessageProcessor* messages;

	std::map<std::string, std::function<void()>> fucntionsOnStart;
	//std::map<std::string, std::function<void()>> functions;

	std::vector<std::function<void()>> functions;
	
	std::string scriptFile;

	std::vector<PlayerInputMessage*> inputMessages;
};

