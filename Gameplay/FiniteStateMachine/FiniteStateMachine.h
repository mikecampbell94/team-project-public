#pragma once

#include "../Gameplay/GameObject.h"
#include "../Gameplay/Scripting/ActionBuilder.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/Scripting/GameLogic.h"

class FiniteStateMachine
{
public:
	FiniteStateMachine();
	~FiniteStateMachine();

	void addGameObjectToFSM(GameObject* gObj)
	{
		gameObjects.push_back(gObj);
	}

	void compileParsedXMLIntoFSM(Node* node);

	void executeFSMMessageBasedActions();
	void executeFSMTimeBasedActions(const float& deltaTime);
	

private:
	std::vector<GameObject*> gameObjects;
	GameLogic logic;
};

