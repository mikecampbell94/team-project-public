#pragma once

#include <string>
#include "InputRecorder.h"

class Player
{

public:
	

	Player() {}
	Player(std::string playerID, InputRecorder *recorder);
	
	~Player();


	void setPlayerID(std::string newID);
	const std::string getPlayerID();

	void setInputRecorder(InputRecorder* newInputRecorder);
	const InputRecorder* getInputRecorder();


private:

	std::string playerID;
	InputRecorder *recorder;
	


};

