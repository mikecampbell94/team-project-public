#include "MoveGameObjectMessage.h"


MoveGameObjectMessage::MoveGameObjectMessage(const std::string& destination, std::string gameObjectID,
	NCLVector3 position)
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
	NCLVector3 position;

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
		else if (childNode->nodeType == "position")
		{
			position = NCLVector3::builder(childNode);
		}
	}

	return MoveGameObjectMessage(destination, object, position);
}

MoveGameObjectMessage MoveGameObjectMessage::tokensToMessage(std::vector<std::string> lineTokens)
{
	if (lineTokens.size() != 4)
	{
		throw std::runtime_error("Incorrect syntax");
	}

	std::string nodeDestination = lineTokens[1];
	std::string nodeResourcename = lineTokens[2];

	std::string positionString = lineTokens[3].substr(9);
	NCLVector3 position = NCLVector3::builder(positionString);

	return MoveGameObjectMessage(nodeDestination, nodeResourcename, position);
}
