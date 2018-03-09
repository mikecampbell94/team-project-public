#include "PaintGameActionBuilder.h"

#include "../../Resource Management/XMLParser.h"
#include "../../Resource Management/Database/Database.h"
#include "../../Communication/Messages/PaintTrailForGameObjectMessage.h"
#include "../../Communication/SendMessageActionBuilder.h"
#include "../GameObject.h"

#include <iostream>

std::unordered_map<std::string, Builder>PaintGameActionBuilder::builders
	= std::unordered_map<std::string, Builder>();

Database* PaintGameActionBuilder::database = nullptr;

void PaintGameActionBuilder::initialiseBuilders(Database* database)
{
	PaintGameActionBuilder::database = database;

	builders.insert({ "PrintText", [](Node* node)
	{
		std::string text = node->children[0]->value;

		return [text]()
		{
			std::cout << text << std::endl;
		};
	} });

	builders.insert({ "CheckPaint", [](Node* node)
	{
		Executable sendMessageAction = SendMessageActionBuilder::buildSendMessageAction(node->children[1]);
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));

		return [sendMessageAction, gameObject]()
		{
			if (gameObject->stats.currentPaint > 0)
			{
				sendMessageAction();
			}
				
		};
	} });

	builders.insert({ "SetMaxPaint", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));
		int maxPaint = stoi(node->children[1]->value);

		return [gameObject, maxPaint]()
		{
			gameObject->stats.maxPaint = maxPaint;
			gameObject->stats.currentPaint = maxPaint;

		};
	} });

	builders.insert({ "ReducePaint", [](Node* node)
	{
		GameObject* gameObject = static_cast<GameObject*>(
			PaintGameActionBuilder::database->getTable("GameObjects")->getResource(node->children[0]->value));

		return [gameObject]()
		{
			int paint = max(--gameObject->stats.currentPaint, 0);
			gameObject->stats.currentPaint = paint;
			
			std::cout << gameObject->getName() << std::endl;

		};
	} });
}

Executable PaintGameActionBuilder::buildExecutable(Node* node)
{
	return builders.at(node->nodeType)(node);
}
