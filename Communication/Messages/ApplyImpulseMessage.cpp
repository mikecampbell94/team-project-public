#include "ApplyImpulseMessage.h"

ApplyImpulseMessage::ApplyImpulseMessage(const std::string& desinationName, std::string gameObjectID, Vector3 impulse) : Message(desinationName, APPLY_IMPULSE)
{
	this->gameObjectID = gameObjectID;
	this->impulse = impulse;
}

ApplyImpulseMessage::~ApplyImpulseMessage()
{
}
