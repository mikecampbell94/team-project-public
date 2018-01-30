#include "System.h"

System::System()
{
}

System::~System()
{
}

void System::updateNextSystemFrame(const float& deltaTime)
{
	for each (std::shared_ptr<Subsystem> subsystem in subsystems)
	{
		subsystem->updateSubsystem(deltaTime);
	}
}

void System::addSubsystem(std::shared_ptr<Subsystem> subsystem)
{
	subsystems.push_back(subsystem);
}
