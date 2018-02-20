#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "Menu.h"

class Mouse;

class UserInterface : public Subsystem
{
public:
	UserInterface(Mouse* mouse, Vector2 resolution, Database* database);
	~UserInterface();

	void updateSubsystem(const float& deltaTime) override;

private:
	Mouse* mouse;
	Vector2 resolution;
	Menu* menu;
};

