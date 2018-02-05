#pragma once
#include "../Utilities/Maths/Vector2.h"
#include <vector>
#include <unordered_map>

enum InputType {
	TRIGGERED,HELD,RELEASED
};

//remove initialiser lists. 
//Move brackets to next line etc
struct ButtonInputData {
	ButtonInputData(InputType currentState, unsigned int key)
	{
		this->currentState = currentState;
		this->key = key;
	};
	InputType currentState;
	unsigned int key;
};

struct LinearInputData {
	LinearInputData(float value, unsigned int key)
	{
		this->value = value;
		this->key = key;	
	}
	float value;
	unsigned int key;

};

struct PositionalInputData {
	PositionalInputData(Vector2 value, unsigned int key)
	{
		this->value = value;
		this->key = key;
	}
	Vector2 value;
	unsigned int key;
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


	std::vector<ButtonInputData> const getInputs();

	std::vector<int> const getKeysToListen();
	

	//addKeyToListenTo and addKeysToListenTo
	void addKeysToListen(std::vector<int> keysToListen);

	void addKeyToListen(int key);

protected:

	std::vector<ButtonInputData> currentButtonInputs;
	std::vector<LinearInputData> currentLinearInputs;
	std::vector<PositionalInputData> currentPositionalInputs;


	std::vector<int> keysToListen;

	playerBase* player;

};

