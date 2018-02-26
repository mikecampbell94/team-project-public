#pragma once

#include "../Gameplay/GameObject.h"

class FiniteStateMachine
{
public:
	FiniteStateMachine();
	~FiniteStateMachine();

	void addGameObjectToFSM(GameObject* gObj)
	{
		gameObjects.push_back(gObj);
	}

private:
	std::vector<GameObject*> gameObjects;
};

