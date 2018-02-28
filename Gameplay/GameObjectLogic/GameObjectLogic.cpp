#include "GameObjectLogic.h"

void changeResource(Node** node, std::string id)
{
	if ((*node)->value == "var")
	{
		(*node)->value = id;
	}
	for each (Node* child in (*node)->children)
	{
		changeResource(&child, id);
	}
}

void changeResourceBack(Node** node, std::string id)
{
	if ((*node)->value == id)
	{
		(*node)->value = "var";
	}
	for each (Node* child in (*node)->children)
	{
		changeResourceBack(&child, id);
	}
}

GameObjectLogic::GameObjectLogic(Database* database, MessageProcessor* messages, std::string script)
{
	this->database = database;
	this->messages = messages;

	XMLParser parser;
	parser.loadFile(script);
	parsedScript = parser.parsedXml;
}

GameObjectLogic::~GameObjectLogic()
{
}

void GameObjectLogic::compileParsedXMLIntoScript()
{
	Node* resources = parsedScript->children[0];
	Node* gameLogicNode = parsedScript->children[1];
	//Node* cStuff = parsedScript->children[2];

	for each (Node* resource in resources->children)
	{
		GameObject* gObj = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(resource->value));

		std::string first = gameLogicNode->children[0]->children[0]->children[0]->children[0]->value;

		changeResource(&(gameLogicNode), resource->value);
		
		std::string second = gameLogicNode->children[0]->children[0]->children[0]->children[0]->value;

		logicToGameObjects.insert({ gObj, GameLogic(messages) });
		logicToGameObjects.at(gObj).compileParsedXMLIntoScript(gameLogicNode);

		changeResourceBack(&(gameLogicNode), resource->value);
		
		std::string third = gameLogicNode->children[0]->children[0]->children[0]->children[0]->value;

		int x = 0;
	}

	for (auto logicToObject = logicToGameObjects.begin(); logicToObject != logicToGameObjects.end(); ++logicToObject)
	{
		logicToObject->second.executeActionsOnStart();
	}
}

void GameObjectLogic::notify(const std::string& messageType, Message* message)
{
	for (auto logicToObject = logicToGameObjects.begin(); logicToObject != logicToGameObjects.end(); ++logicToObject)
	{
		logicToObject->second.notifyMessageActions(messageType, message);
		//notify myself of messages which need to be acted upon via hard coded functions
	}
}

void GameObjectLogic::updatelogic(const float& deltaTime)
{
	for (auto logicToObject = logicToGameObjects.begin(); logicToObject != logicToGameObjects.end(); ++logicToObject)
	{
		logicToObject->second.executeMessageBasedActions();
		logicToObject->second.executeTimeBasedActions(deltaTime);
		logicToObject->second.clearNotifications();

		//update hard coded stuff here
	}
}