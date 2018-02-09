#pragma once

#include <string>
#include "InputFilter.h"


class InputRecorder;


class Player
{

public:
	

	Player() {}
	Player(int playerID, InputRecorder *recorder);
	
	~Player();


	void setPlayerID(int newID);
	const int getPlayerID();

	void setInputRecorder(InputRecorder* newInputRecorder);
	InputRecorder* getInputRecorder();

	void setPlayerControls(InputFilter* newControls);
	InputFilter* getInputFilter();




private:

	int playerID;
	InputRecorder *recorder;
	InputFilter playerControls;


};

