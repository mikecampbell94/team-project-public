#include "GameObjectLogic.h"

GameObjectLogic::GameObjectLogic(Database* database, MessageProcessor* messages)
{
	this->database = database;
	logic = GameLogic(messages);
}

GameObjectLogic::~GameObjectLogic()
{
}

void GameObjectLogic::compileParsedXMLIntoScript(Node* xmlNode)
{
	//child 0 -> is my resource...retrieve it.
	//logic.compileParsedXMLIntoScript(/*child 1*/)
	//compile game object logic with chidl 2

	Node* nodeOne = xmlNode->children[0];
	Node* nodeTwo = xmlNode->children[1];
	Node* nodeThree = xmlNode->children[2];

	gameObject = static_cast<GameObject*>(database->getTable("GameObject")->getResource(nodeOne->value));


	logic.compileParsedXMLIntoScript(nodeTwo);

	
	//Function to compile third node into hard coded c++

	logic.executeActionsOnStart();
}

void notify(message from gameplay subsystem)
{
	logic.notfiy
	notify all hard coded stuff
}

void updatelogic(...)
{
	update logic
	update hardcoded c++ stuff
}