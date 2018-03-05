#include "RotateGameObjectMessage.h"

RotateGameObjectMessage::RotateGameObjectMessage(const std::string& desinationName, std::string gameObjectID,
	Vector4 rotation)
	: Message(desinationName, ROTATE_GAMEOBJECT)
{
	this->gameObjectID = gameObjectID;
	this->rotation = rotation;
}

RotateGameObjectMessage::~RotateGameObjectMessage()
{
}

RotateGameObjectMessage RotateGameObjectMessage::builder(Node* node)
{
	std::string destination = "";
	std::string object = "";
	Vector4 rotation;

	for each (Node* childNode in node->children)
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
			rotation = Vector4::builder(childNode);
		}
	}

	return RotateGameObjectMessage(destination, object, rotation);
}

RotateGameObjectMessage RotateGameObjectMessage::tokensToMessage(std::vector<std::string> lineTokens)
{
	std::string nodeDestination = lineTokens[1];
	std::string nodeResourcename = lineTokens[2];

	std::string rotateString = lineTokens[3].substr(7);
	Vector4 rotate = Vector4::builder(rotateString);

	return RotateGameObjectMessage(nodeDestination, nodeResourcename, rotate);
}
