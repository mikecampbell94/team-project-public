#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "InputGameplay/GameplayInputBridge.h"
#include "Scripting/GameLogic.h"
#include "GameTimer.h"

class GameplaySystem : public Subsystem
{
public:
	explicit GameplaySystem(const int playersInGame);
	~GameplaySystem();

	void updateSubsystem(const float& deltaTime) override;

private:
	GameTimer gameplayTimer;
	GameLogic gameLogic;
	GameplayInputBridge inputBridge;
};

