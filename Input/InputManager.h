#pragma once

#include <vector>
#include "../Launch/Systems/Subsystem.h"
//#include "../ResourceManagement/Resources/Resource.h" doesn't exist yet but may in future?

#include "PlayerBase.h"
#include "Player.h"
#include "../Input/Devices/Window.h"

/*
Manages input devices and their connections to players.
Player 1 will always use M&K. The other 4 will use a controller
*/
class InputManager : public Subsystem, public PlayerBase//, public Resource - uncomment this once resource class is added
{
public:
	InputManager(PlayerBase* playerbase, Window* window);
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
};