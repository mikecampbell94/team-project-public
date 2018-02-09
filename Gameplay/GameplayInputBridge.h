#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include "../Input/Player.h"
#include "../Utilities/Maths/Vector3.h"

typedef std::function<void(Player*, Vector3)> InputAction;
typedef std::unordered_map<int, InputAction> InputMapping;

class GameplayInputBridge
{
public:
	explicit GameplayInputBridge(Player* player);
	~GameplayInputBridge();

	//void assignActionToKey(InputAction action, int inputKey);
	//void executeAction(int newInput);

private:
	InputMapping inputMapping;
	Player* player;
};

