#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "InputGameplay/GameplayInputBridge.h"
#include "Scripting/GameLogic.h"
#include "../Resource Management/XMLParser.h"

class GameplaySystem : public Subsystem
{
public:
	explicit GameplaySystem(const int playersInGame);
	~GameplaySystem();

	void updateSubsystem(const float& deltaTime) override;

private:
	GameLogic gameLogic;
	GameplayInputBridge inputBridge;
	XMLParser inputParser;
};

