#include "RotateGameObjectMessage.h"

RotateGameObjectMessage::RotateGameObjectMessage(const std::string& desinationName, std::string gameObjectID,
	NCLVector4 rotation)
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
	NCLVector4 rotation;

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
			rotation = NCLVector4::builder(childNode);
		}
	}

	return RotateGameObjectMessage(destination, object, rotation);
}

RotateGameObjectMessage RotateGameObjectMessage::tokensToMessage(std::vector<std::string> lineTokens)
{
	std::string nodeDestination = lineTokens[1];
	std::string nodeResourcename = lineTokens[2];

	std::string rotateString = lineTokens[3].substr(7);
	NCLVector4 rotate = NCLVector4::builder(rotateString);

	return RotateGameObjectMessage(nodeDestination, nodeResourcename, rotate);
}
