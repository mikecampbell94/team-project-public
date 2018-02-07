#pragma once

#include "../Communication/MessageProcessor.h"

class Subsystem
{
public:

	Subsystem()
	{
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
	MessageProcessor incomingMessages;
};

