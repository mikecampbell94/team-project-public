#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "Menu.h"

class Keyboard;

class UserInterface : public Subsystem
{
public:
	UserInterface(Keyboard* mouse, Vector2 resolution);
	~UserInterface();

	void initialise(std::string menuFile, Database* database);

	void updateSubsystem(const float& deltaTime) override;

private:
	Keyboard* keyboard;
	Vector2 resolution;
	Menu* menu = nullptr;
	bool interfaceDisplaying = true;
};

