#pragma once
#include <vector>
#include <unordered_map>

enum state {
	TRIGGERED,HELD,RELEASED
};

//remove initialiser lists. 
//Move brackets to next line etc
struct KeyState {
	KeyState(state currentState, int key) : currentState(currentState), key(key) {};
	state currentState;
	int key;
};


//Remove playerbase from here
class playerBase;

class InputRecorder
{
public:
	//take vector<int> instead
	InputRecorder() {};
	~InputRecorder() {};


	virtual void fillInputs() = 0;
	virtual void clearInputs() = 0;


	std::vector<KeyState> const getInputs() { return currentInputs; };
	std::vector<int> const getKeysToListen() { return keysToListen; };

	//addKeyToListenTo and addKeysToListenTo
	void addKeysToListen(std::vector<int> keysToListen) { this->keysToListen = keysToListen; };
	void addKeyToListen(int key) { this->keysToListen.push_back(key); };

protected:
	std::vector<KeyState> currentInputs;
	std::vector<int> keysToListen;

	playerBase* player;

};

