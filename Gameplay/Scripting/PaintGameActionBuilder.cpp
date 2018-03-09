#include "PaintGameActionBuilder.h"

#include "../Resource Management/XMLParser.h"
#include "../../Communication/Messages/PaintTrailForGameObjectMessage.h"
#include "../../Communication/SendMessageActionBuilder.h"
#include "../GameObject.h"

#include <iostream>

std::unordered_map<std::string, Builder>PaintGameActionBuilder::builders
	= std::unordered_map<std::string, Builder>();

void PaintGameActionBuilder::initialiseBuilders()
{
	builders.insert({ "PrintText", [](Node* node)
	{
		std::string text = node->children[0]->value;

		return [text]()
		{
			std::cout << text << std::endl;
		};
	} });

	/*builders.insert({ "CheckPaint", [](Node* node)
	{
		Executable sendMessageAction = SendMessageActionBuilder::buildSendMessageAction(node->children[1]);
		GameObject* gameObject = static_cast<GameObject*>(
			database->getTable("GameObjects")->getResource(node->children[0]->value));
		
		get playter from var;


		return [sendMessageAction]()
		{
			if(player.paint > 0)
				sendMessageAction();
		};
	} });*/
}

Executable PaintGameActionBuilder::buildExecutable(Node* node)
{
	return builders.at(node->nodeType)(node);
}
