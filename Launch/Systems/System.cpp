#include "System.h"

#include "Communication/DeliverySystem.h"
#include "Communication/LetterBox.h"
#include <iostream>

System::System()
{
	letterBox = new LetterBox();
	DeliverySystem::provide(letterBox);
}

System::~System()
{
}

void System::updateNextSystemFrame(const float& deltaTime)
{
	for (Subsystem* subsystem : subsystems)
	{
		subsystem->updateSubsystem(deltaTime);
	}
}

void System::processMessagesForAllSubsystems()
{
	for (Subsystem* subsystem : subsystems)
	{
		subsystem->processMessages();
	}
}

void System::addSubsystem(Subsystem* subsystem)
{
	subsystems.push_back(subsystem);
}

std::vector<Subsystem*> System::getSubSystems()
{
	return subsystems;
}
