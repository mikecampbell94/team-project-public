#include "System.h"

#include "Communication/DeliverySystem.h"
#include "Communication/LetterBox.h"
#include <iostream>
#include <ctime>
#include <omp.h>  

System::System()
{
	DeliverySystem::provide(new LetterBox());
	omp_set_num_threads(omp_get_num_procs() - 1);
	//omp_set_dynamic(0);
}

System::~System()
{
}

void System::updateNextSystemFrame(const float& deltaTime)
{
	std::clock_t start = std::clock();
	rendering->updateSubsystem(deltaTime);
	rendering->synchronise();
	std::clock_t end = std::clock();

	std::clock_t startF = std::clock();
	#pragma omp parallel for
		for (int i = 0; i < subsystems.size(); ++i)
		{
			std::cout << "Thread ID: " << omp_get_thread_num() << std::endl;
			subsystems[i]->updateSubsystem(deltaTime);
		}
	#pragma omp barrier

	std::clock_t endF = std::clock();
	//std::cout << "Loop: " << ((float)(endF - startF)) / CLOCKS_PER_SEC << "	Renderer:	" << ((float)(end - start)) / CLOCKS_PER_SEC << std::endl;
}

void System::processMessagesForAllSubsystems()
{
	for each (Subsystem* subsystem in subsystems)
	{
		subsystem->processMessages();
	}

	rendering->processMessages();
}

void System::addSubsystem(Subsystem* subsystem)
{
	if (subsystem->getSubsystemName() == "RenderingSystem")
	{
		rendering = static_cast<RenderingSystem*>(subsystem);
	}
	else
	{
		subsystems.push_back(subsystem);
	}
}
