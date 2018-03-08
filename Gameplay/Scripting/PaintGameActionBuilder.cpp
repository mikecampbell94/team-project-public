#include "PaintGameActionBuilder.h"

#include "../Resource Management/XMLParser.h"

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
}

Executable PaintGameActionBuilder::buildExecutable(Node* node)
{
	return builders.at(node->nodeType)(node);
}
