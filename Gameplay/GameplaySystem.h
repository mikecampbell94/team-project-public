#pragma once

#include "../Launch/Systems/Subsystem.h"
#include "InputGameplay/GameplayInputBridge.h"
#include "Scripting/GameLogic.h"
#include "../Resource Management/XMLParser.h"
#include "FiniteStateMachine/FiniteStateMachine.h"

class GameplaySystem : public Subsystem
{
public:
	GameplaySystem();
	~GameplaySystem();

	void updateSubsystem(const float& deltaTime) override;

	void connectPlayerbase(PlayerBase* playerbase);
	void compileGameplayScript(std::string levelScript);
	void compileFSMScript(std::string fsmScript);

private:
	GameLogic gameLogic;
	FiniteStateMachine fsm;
	GameplayInputBridge inputBridge;
	XMLParser inputParser;
};

