#include "GameObjectLogic.h"

void changeResource(Node** node, std::string id)
{
	if((*node)->nodeType == "resource" || (*node)->nodeType == "objectIdentifier")
	{
		if((*node)->value == "var")
		{
			(*node)->value = id;
			return;
		}
		
	}
	for each (Node* child in (*node)->children)
	{
		changeResource(&child, id);
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

void GameObjectLogic::compileResources(Node* node)
{
	
}




void GameObjectLogic::compileParsedXMLIntoScript()
{
	Node* resources = parsedScript->children[0];
	
	//Node* cStuff = parsedScript->children[2];

	for each (Node* resource in resources->children)
	{
		GameObject* gObj = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(resource->value));

		Node* gameLogic = parsedScript->children[1];

		changeResource(&(gameLogic), resource->value);
		
		logicToGameObjects[gObj] = GameLogic(messages);
		logicToGameObjects[gObj].compileParsedXMLIntoScript(gameLogic);
	}

	//iterate map
	//logicToGameObjects[gObj].executeActionsOnStart();
}

void GameObjectLogic::notify(const std::string& messageType, Message* message)
{
	//logic.notifyMessageActions(messageType, message);
	//notify myself of messages which need to be acted upon via hard coded functions
}

void GameObjectLogic::updatelogic(const float& deltaTime)
{
	/*logic.executeMessageBasedActions();
	logic.executeTimeBasedActions(deltaTime);
	logic.clearNotifications();*/
}