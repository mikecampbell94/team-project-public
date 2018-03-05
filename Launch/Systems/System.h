#pragma once

#include "Subsystem.h"
#include "../Communication/MessageStorage.h"

#include <vector>
#include <memory>

class LetterBox;

class System
{
public:
	System();
	~System();

	void updateNextSystemFrame(const float& deltaTime);
	void processMessagesForAllSubsystems();

	void addSubsystem(Subsystem* subsystem);
	std::vector<Subsystem*> getSubSystems();

private:
	std::vector<Subsystem*> subsystems;
	MessageStorage messageBuffers;
	LetterBox* letterBox;
};

