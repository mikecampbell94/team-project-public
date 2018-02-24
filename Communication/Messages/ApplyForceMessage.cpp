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
