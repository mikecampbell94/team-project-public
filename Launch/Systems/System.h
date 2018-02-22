#pragma once

#include "Subsystem.h"
#include "../Communication/MessageStorage.h"

#include <vector>
#include <memory>
#include "Rendering/RenderingSystem.h"

class System
{
public:
	System();
	~System();

	void updateNextSystemFrame(const float& deltaTime);
	void processMessagesForAllSubsystems();

	void addSubsystem(Subsystem* subsystem);

private:
	std::vector<Subsystem*> subsystems;
	MessageStorage messageBuffers;

	RenderingSystem* rendering;
};

