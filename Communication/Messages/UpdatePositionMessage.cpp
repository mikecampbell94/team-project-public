#include "UpdatePositionMessage.h"

UpdatePositionMessage::UpdatePositionMessage(const std::string& desinationName, const std::string& gameObjectID, const Vector3& position)
	: Message(desinationName, UPDATE_POSITION)
{
	this->gameObjectID = gameObjectID;
	this->position = position;
}

UpdatePositionMessage::~UpdatePositionMessage()
{
}
