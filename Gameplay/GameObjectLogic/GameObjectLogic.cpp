#include "GameObjectLogic.h"

GameObjectLogic::GameObjectLogic(Database* database)
{
	this->database = database;
}

GameObjectLogic::~GameObjectLogic()
{
}

void GameObjectLogic::compileParsedXMLIntoScript(Node* xmlNode)
{
	for each (Node* node in xmlNode->children)
	{
		if (node->nodeType == "OnStart")
		{
			gameObject = static_cast<GameObject*>(database->getTable("GameObjects")->getResource())
		}
	}
}