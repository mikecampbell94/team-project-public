#pragma once

#include "../../Communication/Messages/PlayerInputMessage.h"
#include "InputActionMap.h"

class GameplayInputBridge
{
public:
	GameplayInputBridge() = default;
	explicit GameplayInputBridge(const int maxPlayers);
	~GameplayInputBridge();

	void addInputActionMapForPlayer(const InputActionMap& mapping);
	void processPlayerInputMessage(const PlayerInputMessage& message);

private:
	std::vector<InputActionMap> actionsForEachPlayer;
};

