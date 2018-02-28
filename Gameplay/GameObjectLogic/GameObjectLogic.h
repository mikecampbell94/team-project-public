#pragma once

#include "../Communication/Message.h"
#include "../Gameplay/GameObject.h"
#include "../Gameplay/Scripting/ActionBuilder.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/Scripting/GameLogic.h"

class Database;

class GameObjectLogic
{
public:
	GameObjectLogic(Database* database, MessageProcessor* messages, std::string script);
	~GameObjectLogic();

	void compileParsedXMLIntoScript();
	void notify(const std::string& messageType, Message* message);
	void updatelogic(const float& deltaTime);
	

private:
	void compileResources(Node* node);
	void compileFunctions(Node* node);

	Node* parsedScript;
	//GameLogic logic;
	std::map<GameObject*, GameLogic> logicToGameObjects;
	Database* database;
	MessageProcessor* messages;
};

