#pragma once

#include <string>



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
	const InputRecorder* getInputRecorder();


private:

	int playerID;
	InputRecorder *recorder;
	


};

