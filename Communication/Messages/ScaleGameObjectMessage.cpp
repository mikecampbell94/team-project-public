#include "ScaleGameObjectMessage.h"

ScaleGameObjectMessage::ScaleGameObjectMessage(const std::string& desinationName, std::string gameObjectID,
	Vector3 scale)
	: Message(desinationName, SCALE_GAMEOBJECT)
{
	this->gameObjectID = gameObjectID;
	this->scale = scale;
}

ScaleGameObjectMessage::~ScaleGameObjectMessage()
{
}

ScaleGameObjectMessage ScaleGameObjectMessage::builder(Node* node)
{
	std::string destination = "";
	std::string object = "";
	Vector3 scale;

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
			scale = Vector3::builder(childNode);
		}
	}

	return ScaleGameObjectMessage(destination, object, scale);
}

ScaleGameObjectMessage ScaleGameObjectMessage::tokensToMessage(std::vector<std::string> lineTokens)
{
	std::string nodeDestination = lineTokens[1];
	std::string nodeResourcename = lineTokens[2];

	std::string scaleString = lineTokens[3].substr(6);
	Vector3 scale = Vector3::builder(scaleString);

	return ScaleGameObjectMessage(nodeDestination, nodeResourcename, scale);
}
