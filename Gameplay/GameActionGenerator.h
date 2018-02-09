#pragma once
#include <vector>
#include <functional>

class Player;
class Vector3;

enum Action
{
	TRANSLATE,
	APPLY_FORCE
};

class GameActionGenerator
{
public:
	GameActionGenerator();
	~GameActionGenerator();

	//std::function<void(Player*, Vector3)> CreateActionExecutable();

private:
	//std::vector<std::function<void(Player*, Vector3)>> actions;

	//static void Translate(Player* player, Vector3 position);
	//static void ApplyForce(Player* player, Vector3 force);
};

