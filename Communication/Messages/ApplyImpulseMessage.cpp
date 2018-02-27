#include "ApplyImpulseMessage.h"

ApplyImpulseMessage::ApplyImpulseMessage(const std::string& desinationName, std::string gameObjectID, Vector3 impulse) : Message(desinationName, APPLY_IMPULSE)
{
	this->gameObjectID = gameObjectID;
	this->impulse = impulse;
}

ApplyImpulseMessage::~ApplyImpulseMessage()
{
}

ApplyImpulseMessage ApplyImpulseMessage::builder(Node* node)
{
	std::string destination = "";
	std::string object = "";
	Vector3 impulse;

	for each (Node* childNode in node->children)
	{
		if(childNode->nodeType == destination)
		{
			destination = childNode->value;
		}
		else if (childNode->nodeType == "gameObjectID")
		{
			object = childNode->value;
		}
		else if (childNode->nodeType == "impulse")
		{
			impulse.x = std::stof(childNode->children[0]->value);
			impulse.y = std::stof(childNode->children[1]->value);
			impulse.z = std::stof(childNode->children[2]->value);
		}
	}

	return ApplyImpulseMessage(destination, object, impulse);
}

