#pragma once

#include "../Launch/Systems/Subsystem.h"

class GameplaySystem : public Subsystem
{
public:
	GameplaySystem();
	~GameplaySystem();

	void updateSubsystem(const float& deltaTime) override;
};

