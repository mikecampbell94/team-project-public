#include "System.h"

System::System()
{
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
