#pragma once

#include "Subsystem.h"

#include <vector>
#include <memory>

class System
{
public:
	System();
	~System();

	void updateNextSystemFrame(const float& deltaTime);
	void addSubsystem(std::shared_ptr<Subsystem> subsystem);

private:
	std::vector<std::shared_ptr<Subsystem>> subsystems;
};

