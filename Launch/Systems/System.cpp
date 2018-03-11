#include "System.h"

#include "Communication/DeliverySystem.h"
#include "Communication/LetterBox.h"
#include <iostream>
#include <ctime>

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
	auto start = std::clock();

	vector<TaskFuture<void>> updates;

	for (Subsystem* subsystem : subsystems)
	{
		updates.push_back(threadPool.submitJob([](float deltaTime, Subsystem* subsystem)
		{
			subsystem->updateSubsystem(deltaTime);
		}, deltaTime, subsystem));
	}

	renderer->updateSubsystem(deltaTime);

	for (auto& task : updates)
	{
		task.Complete();
	}

	auto end = std::clock();

	std::cout << end - start << std::endl;
}

void System::addSubsystem(Subsystem* subsystem)
{
	subsystems.push_back(subsystem);
}

std::vector<Subsystem*> System::getSubSystems()
{
	return subsystems;
}
