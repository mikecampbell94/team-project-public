#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "Menu.h"

class Keyboard;

class UserInterface : public Subsystem
{
public:
	UserInterface(Keyboard* mouse, NCLVector2 resolution);
	~UserInterface();

	void initialise(std::string menuFile, Database* database);

	void updateNextFrame(const float& deltaTime) override;

private:
	void toggleModule();

	Keyboard* keyboard;
	NCLVector2 resolution;
	Menu* menu = nullptr;
	bool enabled = true;
	bool blocked;
};

