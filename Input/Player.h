#pragma once

#include <string>
#include "InputRecorder.h"

class Player
{

public:
	

	Player() {}
	Player(int playerID, InputRecorder *recorder);
	
	~Player();


	void setPlayerID(int newID);
	const int getPlayerID();

	void setInputRecorder(InputRecorder* newInputRecorder);
	const InputRecorder* getInputRecorder();


private:

	int playerID;
	InputRecorder *recorder;
	


};

