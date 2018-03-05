#pragma once

#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

class GameTimer;

class Subsystem
{
public:

	Subsystem(std::string subsystemName);

	virtual ~Subsystem();

	virtual void updateSubsystem(const float& deltaTime = 0.0f) = 0;

	void processMessages();

	GameTimer* getTimer();
	std::string getSubsystemName();

protected:
	std::string subsystemName;
	MessageProcessor incomingMessages;
	GameTimer* timer;
};

