#pragma once

#include "Subsystem.h"
#include "../Communication/MessageStorage.h"

#include <vector>
#include <memory>
#include "Launch/ThreadPool/ThreadPool.h"
#include "Rendering/RenderingSystem.h"

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

	RenderingSystem* renderer;

private:
	std::vector<Subsystem*> subsystems;
	MessageStorage messageBuffers;
	LetterBox* letterBox;
	ThreadPool threadPool;
};

