#pragma once
#include <vector>
#include <unordered_map>

enum state {
	TRIGGERED,HELD,RELEASED
};

struct KeyState {
	KeyState(state currentState, int key) : currentState(currentState), key(key) {};
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
	virtual void clearInputs() = 0;


	std::vector<KeyState> const getInputs() { return currentInputs; };
	std::vector<int> const getKeysToListen() { return keysToListen; };

	void setKeysToListen(std::vector<int> keysToListen) { this->keysToListen = keysToListen; };
	void addKeyToListen(int key) { this->keysToListen.push_back(key); };

protected:
	std::vector<KeyState> currentInputs;
	std::vector<int> keysToListen;

	playerBase* player;

};

