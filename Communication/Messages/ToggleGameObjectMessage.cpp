#include "ToggleGameObjectMessage.h"



ToggleGameObjectMessage::ToggleGameObjectMessage(const std::string& destinationName, std::string gameObjectID,
	bool isEnabled)
	: Message(destinationName, TOGGLE_GAMEOBJECT)
{
	this->gameObjectID = gameObjectID;
	this->isEnabled = isEnabled;
}


ToggleGameObjectMessage::~ToggleGameObjectMessage()
{
}

ToggleGameObjectMessage ToggleGameObjectMessage::builder(Node* node)
{
	std::string destination = "";
	std::string object = "";
	bool isEnabled;

	for (Node* childNode : node->children)
	{
		if (childNode->nodeType == "destination")
		{
			destination = childNode->value;
		}
		else if (childNode->nodeType == "resource")
		{
			object = childNode->value;
		}
		else if (childNode->nodeType == "scale")
		{
			isEnabled = NCLVector4::builder(childNode);
		}
	}
	return ToggleGameObjectMessage(destination, object, isEnabled);
}

ToggleGameObjectMessage ToggleGameObjectMessage::tokensToMessage(std::vector<std::string> lineTokens)
{
	std::string nodeDestination = lineTokens[1];
	std::string nodeResourcename = lineTokens[2];

	std::string rotateString = lineTokens[3].substr(7);
	bool isEnabled = NCLVector4::builder(rotateString);

	return ToggleGameObjectMessage(nodeDestination, nodeResourcename, isEnabled);
}