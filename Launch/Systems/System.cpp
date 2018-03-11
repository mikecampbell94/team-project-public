#include "System.h"

#include "Communication/DeliverySystem.h"
#include "Communication/LetterBox.h"
#include <iostream>
#include <chrono>

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
	vector<TaskFuture<void>> updates;

	for (Subsystem* subsystem : subsystems)
	{
		updates.push_back(threadPool.SubmitJob([deltaTime, subsystem]()
		{
			subsystem->updateSubsystem(deltaTime);
		}));
	}

	renderer->updateSubsystem(deltaTime);

	for (auto& task : updates)
	{
		task.Complete();
	}
}

void System::processMessagesForAllSubsystems()
{
	renderer->processMessages();

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
