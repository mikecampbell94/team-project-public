#include "GameplayInputBridge.h"

GameplayInputBridge::GameplayInputBridge(const int maxPlayers)
{
	//actionsForEachPlayer = std::vector<InputActionMap>(maxPlayers);
}

GameplayInputBridge::~GameplayInputBridge()
{
}

void GameplayInputBridge::addInputActionMapForPlayer(const InputActionMap& mapping)
{
	//actionsForEachPlayer[mapping.getAssociatedPlayerID()] = mapping;
	actionsForEachPlayer.insert({mapping.getAssociatedPlayerID(), mapping});
}

void GameplayInputBridge::processPlayerInputMessage(const PlayerInputMessage& message)
{
	actionsForEachPlayer.at(message.player->getPlayerID()).executeAction(message.data.key, message.player);
}
