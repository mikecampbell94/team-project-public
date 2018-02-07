#include "System.h"

#include "Communication/DeliverySystem.h"

System::System()
{
	DeliverySystem::provide(new MessageStorage());
}

System::~System()
{
}

void System::updateNextSystemFrame(const float& deltaTime)
{
	for each (Subsystem* subsystem in subsystems)
	{
		subsystem->updateSubsystem(deltaTime);
	}
}

void System::addSubsystem(Subsystem* subsystem)
{
	subsystems.push_back(subsystem);
}
