#pragma once
#include "../Utilities/Maths/Vector2.h"
#include "../Players/PlayerBase.h"
#include <vector>
#include <unordered_map>

enum InputType {
	TRIGGERED,HELD,RELEASED, NONE
};

//remove initialiser lists. 
//Move brackets to next line etc
struct ButtonInputData {
	ButtonInputData(const InputType currentState, const unsigned int key)
	{
		this->currentState = currentState;
		this->key = key;
	}

	ButtonInputData()
	{
		this->currentState = NONE;
		this->key = 0;
	}

	InputType currentState;
	unsigned int key;
};

struct LinearInputData {
	LinearInputData(const float value, const std::string key)
	{
		this->value = value;
		this->key = key;	
	}
	float value;
	std::string key;

};

struct PositionalInputData {
	PositionalInputData(const Vector2 value, const std::string key)
	{
		this->value = value;
		this->key = key;
	}
	Vector2 value;
	std::string key;
};

class InputRecorder
{
public:
	explicit InputRecorder(std::vector<int> keysToListen) {};
	InputRecorder() {}
	~InputRecorder() {}

	virtual void fillInputs() = 0;
	virtual void clearInputs();

	std::vector<ButtonInputData> const getInputs();
	std::vector<int> const getKeysToListen();
	
	void addKeysToListen(std::vector<int> keysToListen);
	void removeListenedKey(int key);//TODO
	void addKeyToListen(int key);

protected:
	std::vector<ButtonInputData> currentButtonInputs;
	std::vector<LinearInputData> currentLinearInputs;
	std::vector<PositionalInputData> currentPositionalInputs;

	std::vector<int> keysToListen;
	PlayerBase* player;
};

