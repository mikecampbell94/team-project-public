#pragma once

#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

class Subsystem
{
public:

	Subsystem(std::string subsystemName)
	{
		this->subsystemName = subsystemName;
		DeliverySystem::getPostman()->addDeliveryPoint(subsystemName);
	}

	virtual ~Subsystem()
	{
	}

	virtual void updateSubsystem(const float& deltaTime = 0.0f) = 0;

	void processMessages()
	{
		incomingMessages.processMessagesInBuffer();
	}

	std::string getSubsystemName()
	{
		return subsystemName;
	}

protected:
	std::string subsystemName;
	MessageProcessor incomingMessages;
};

