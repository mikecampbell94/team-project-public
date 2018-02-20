#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "Menu.h"

class Keyboard;

class UserInterface : public Subsystem
{
public:
	UserInterface(Keyboard* mouse, Vector2 resolution, Database* database);
	~UserInterface();

	void updateSubsystem(const float& deltaTime) override;

private:
	Keyboard* keyboard;
	Vector2 resolution;
	Menu* menu;
};

