#pragma once
#include <vector>

enum keyState {
	TRIGGERED,HELD,RELEASED
};




class playerBase;

class InputRecorder
{
public:
	InputRecorder(playerBase* pb) : player(pb){};
	~InputRecorder() {};


	virtual void fillInputs() = 0;
	virtual void cleanInputs() = 0;


	std::vector<int> keysToListen;


protected:
	

	playerBase* player;

};

