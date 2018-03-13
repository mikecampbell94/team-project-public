#include "System.h"

#include "Communication/DeliverySystem.h"
#include "Communication/LetterBox.h"
#include <iostream>
#include <ctime>

System::System(ThreadPool* threadPool)
{
	letterBox = new LetterBox();
	DeliverySystem::provide(letterBox);
	this->threadPool = threadPool;
}

System::~System()
{
}

void System::updateNextSystemFrame(const float& deltaTime)
{
	auto start = std::clock();

	vector<TaskFuture<void>> updates;

	for (Subsystem* subsystem : concurrentSubsystems)
	{
		updates.push_back(threadPool->submitJob([](float deltaTime, Subsystem* subsystem)
		{
			subsystem->updateSubsystem(deltaTime);
		}, deltaTime, subsystem));
	}

	for (Subsystem* subsystem : subsystems)
	{
		subsystem->updateSubsystem(deltaTime);
	}

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

void System::addConcurrentSubsystem(Subsystem* subsystem)
{
	concurrentSubsystems.push_back(subsystem);
}

void System::processAllMessages()
{
	//for (Subsystem* subsystem : concurrentSubsystems)
	//{
	//	subsystem->processMessages();
	//}

	//for (Subsystem* subsystem : subsystems)
	//{
	//	subsystem->processMessages();
	//}
}

std::vector<Subsystem*> System::getSubSystems()
{
	vector<Subsystem*> allSubsystems;
	allSubsystems.insert(std::end(allSubsystems), std::begin(subsystems), std::end(subsystems));
	allSubsystems.insert(std::end(allSubsystems), std::begin(concurrentSubsystems), std::end(concurrentSubsystems));

	return allSubsystems;
}
