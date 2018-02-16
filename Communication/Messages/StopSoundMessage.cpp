#include "StopSoundMessage.h"

StopSoundMessage::StopSoundMessage(const std::string& desinationName, MessageType type, std::string soundNodeIdentifier) : Message(desinationName, type)
{
	this->soundNodeIdentifier = soundNodeIdentifier;
}

StopSoundMessage::~StopSoundMessage()
{
}
