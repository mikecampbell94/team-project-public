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

	void clearInputActions()
	{
		actionsForEachPlayer.clear();
	}

private:
	//std::vector<InputActionMap> actionsForEachPlayer;
	std::unordered_map<int, InputActionMap> actionsForEachPlayer;
};

