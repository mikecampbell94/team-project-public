#include "ApplyForceMessage.h"

ApplyForceMessage::ApplyForceMessage(const std::string& desinationName, const std::string& gameObjectID, const Vector3& force)
	: Message(desinationName, APPLY_FORCE)
{
	this->gameObjectID = gameObjectID;
	this->force = force;
}

ApplyForceMessage::~ApplyForceMessage()
{
}

ApplyForceMessage ApplyForceMessage::builder(Node* node)
{
	std::string destination = "";
	std::string object = "";
	Vector3 force;

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
		else if (childNode->nodeType == "force")
		{
			force.x = std::stof(childNode->children[0]->value);
			force.y = std::stof(childNode->children[1]->value);
			force.z = std::stof(childNode->children[2]->value);
		}
	}

	return ApplyForceMessage(destination, object, force);
}
