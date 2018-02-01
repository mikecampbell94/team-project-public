#pragma once
#include <vector>
#include <unordered_map>

enum state {
	TRIGGERED,HELD,RELEASED
};

struct keyState {
	state currentState;
	int key;
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
	std::unordered_map<int, keyState> currentInputs;


	playerBase* player;

};

