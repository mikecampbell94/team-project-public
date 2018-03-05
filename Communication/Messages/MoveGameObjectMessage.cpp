#include "MoveGameObjectMessage.h"


MoveGameObjectMessage::MoveGameObjectMessage(const std::string& destination, std::string gameObjectID,
	Vector3 position)
	: Message(destination, MOVE_GAMEOBJECT)
{
	this->gameObjectID = gameObjectID;
	this->position = position;
}

MoveGameObjectMessage::~MoveGameObjectMessage()
{
}

MoveGameObjectMessage MoveGameObjectMessage::builder(Node* node)
{
	std::string destination = "";
	std::string object = "";
	Vector3 position;

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
		else if (childNode->nodeType == "position")
		{
			position = Vector3::builder(childNode);
		}
	}

	return MoveGameObjectMessage(destination, object, position);
}

MoveGameObjectMessage MoveGameObjectMessage::tokensToMessage(std::vector<std::string> lineTokens)
{
	std::string nodeDestination = lineTokens[1];
	std::string nodeResourcename = lineTokens[2];

	std::string positionString = lineTokens[3].substr(9);
	Vector3 position = Vector3::builder(positionString);

	return MoveGameObjectMessage(nodeDestination, nodeResourcename, position);
}
