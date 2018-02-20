#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "InputGameplay/GameplayInputBridge.h"
#include "Scripting/GameLogic.h"

class GameplaySystem : public Subsystem
{
public:
	explicit GameplaySystem(const int playersInGame);
	~GameplaySystem();

	void updateSubsystem(const float& deltaTime) override;

	void compileGameplayScript(std::string levelScript);

private:
	GameLogic gameLogic;
	GameplayInputBridge inputBridge;
};

