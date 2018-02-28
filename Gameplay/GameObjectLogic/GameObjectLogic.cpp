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
		logics.push_back(&(logicToGameObjects.at(gObj)));

		logics[logics.size() - 1]->compileParsedXMLIntoScript(gameLogicNode);

		changeResourceBack(&(gameLogicNode), resource->value);
		
		std::string third = gameLogicNode->children[0]->children[0]->children[0]->children[0]->value;

		int x = 0;
	}

	//for (auto logicToObject = logicToGameObjects.begin(); logicToObject != logicToGameObjects.end(); ++logicToObject)
	//{
	//	logicToObject->second->executeActionsOnStart();
	//}

	for each (GameLogic* logic in logics)
	{
		logic->executeActionsOnStart();
	}
}

void GameObjectLogic::notify(const std::string& messageType, Message* message)
{
	//for (auto logicToObject = logicToGameObjects.begin(); logicToObject != logicToGameObjects.end(); ++logicToObject)
	//{
	//	logicToObject->second->notifyMessageActions(messageType, message);
	//	//notify myself of messages which need to be acted upon via hard coded functions
	//}

	for each (GameLogic* logic in logics)
	{
		logic->notifyMessageActions(messageType, message);
	}
}

void GameObjectLogic::updatelogic(const float& deltaTime)
{
	//for (auto logicToObject = logicToGameObjects.begin(); logicToObject != logicToGameObjects.end(); ++logicToObject)
	//{
	//	logicToObject->second->executeMessageBasedActions();
	//	logicToObject->second->executeTimeBasedActions(deltaTime);
	//	logicToObject->second->clearNotifications();

	//	//update hard coded stuff here
	//}

	for each (GameLogic* logic in logics)
	{
		logic->executeMessageBasedActions();
		logic->executeTimeBasedActions(deltaTime);
		logic->clearNotifications();
	}
}