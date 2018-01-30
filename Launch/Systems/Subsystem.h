#pragma once

class Subsystem
{
public:

	Subsystem()
	{
	}

	virtual ~Subsystem()
	{
	}

	virtual void updateSubsystem(const float& deltaTime = 0.0f) = 0;
};

