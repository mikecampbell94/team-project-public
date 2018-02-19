#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "UserInterfaceBuilder.h"

class Mouse;

class UserInterface : public Subsystem
{
public:
	UserInterface(Mouse* mouse, Vector2 resolution);
	~UserInterface();

	void updateSubsystem(const float& deltaTime) override;

private:
	Mouse* mouse;
	Vector2 resolution;

	Button* button;
};

