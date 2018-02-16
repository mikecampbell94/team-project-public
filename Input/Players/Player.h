#pragma once

#include <string>
#include "InputFilter.h"
#include "../Graphics/Scene Management/SceneNode.h"

class InputRecorder;

class Player
{

public:
	Player() = default;
	Player(int playerID, InputRecorder *recorder/*, SceneNode* playerNode*/);
	~Player();

	void setPlayerID(int newID);
	const int getPlayerID();

	void setInputRecorder(InputRecorder* newInputRecorder);
	InputRecorder* getInputRecorder();

	void setPlayerControls(InputFilter* newControls);
	InputFilter* getInputFilter();

	void setSceneNode(SceneNode* node);
	SceneNode* getSceneNode();

private:
	SceneNode* node;
	int playerID;
	InputRecorder *recorder;
	InputFilter playerControls;


};

