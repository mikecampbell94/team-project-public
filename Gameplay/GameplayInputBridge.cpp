#include "GameplayInputBridge.h"

GameplayInputBridge::GameplayInputBridge(Player* player)
{
	this->player = player;
}

GameplayInputBridge::~GameplayInputBridge()
{
}

//void GameplayInputBridge::assignActionToKey(InputAction action, int inputKey)
//{
//	inputMapping.insert({ inputKey , action });
//}
//
//void GameplayInputBridge::executeAction(int newInput)
//{
//	inputMapping.at(newInput)(player);
//}
