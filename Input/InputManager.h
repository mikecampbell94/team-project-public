#pragma once

#include <vector>
#include "../Launch/Systems/Subsystem.h"
//#include "../ResourceManagement/Resources/Resource.h" doesn't exist yet but may in future?

#include "Players/PlayerBase.h"
#include "Players/Player.h"
#include "../Input/Devices/Window.h"
#include "InputControl.h"


class InputManager : public Subsystem//, public Resource - uncomment this once resource class is added
{
public:
	InputManager(PlayerBase* playerbase);
	~InputManager();

	void updateSubsystem(const float& deltatime) override;

	PlayerBase* GetPlayerbase() const
	{
		return playerbase;
	}

	//void Read(const string resourcename) override; - uncomment this once resource class is added
	//void ReadParams(const string params) override; - uncomment this once resource class is added

private:
	PlayerBase* playerbase;
	Window* window;
	InputControl inputControl;

	bool blocked;
};
