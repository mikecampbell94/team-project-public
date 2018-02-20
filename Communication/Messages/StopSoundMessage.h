#pragma once

#include "../Communication/Message.h"
#include "../Audio/Sound.h"
#include "../Audio/SoundNode.h"

class StopSoundMessage : public Message
{
public:
	StopSoundMessage(const std::string& desinationName, MessageType type, std::string soundNodeIdentifier);
	~StopSoundMessage();

	std::string soundNodeIdentifier;
};

