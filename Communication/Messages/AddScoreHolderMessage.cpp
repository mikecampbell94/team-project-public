#include "AddScoreHolderMessage.h"

AddScoreHolderMessage::AddScoreHolderMessage(const std::string& desinationName, const std::string& name)
	: Message(desinationName, ADD_SCORE_HOLDER)
{
	this->name = name;
}

AddScoreHolderMessage::~AddScoreHolderMessage()
{
}

AddScoreHolderMessage AddScoreHolderMessage::builder(Node* node)
{
	std::string nodeDestination = "";
	std::string nodeResourceName = "";

	for each (Node* childNode in node->children)
	{
		if (childNode->nodeType == "destination")
		{
			nodeDestination = childNode->value;
		}
		else if (childNode->nodeType == "name")
		{
			nodeResourceName = childNode->value;
		}
	}

	return AddScoreHolderMessage(nodeDestination, nodeResourceName);
}
