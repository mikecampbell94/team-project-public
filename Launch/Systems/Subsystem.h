#pragma once

#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

class Subsystem
{
public:

	Subsystem(std::string subsystemName)
	{
		this->subsystemName = subsystemName;
		DeliverySystem::getPostman()->addMessageBuffer(subsystemName);
	}

	virtual ~Subsystem()
	{
	}

	virtual void updateSubsystem(const float& deltaTime = 0.0f) = 0;

	void processMessages()
	{
		incomingMessages.processMessagesInBuffer();
	}

protected:
	std::string subsystemName;
	MessageProcessor incomingMessages;
};

