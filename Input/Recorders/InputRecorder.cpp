#include "InputRecorder.h"

void InputRecorder::clearInputs()
{
	currentButtonInputs.clear();
	currentLinearInputs.clear();
	currentPositionalInputs.clear();
}

std::vector<ButtonInputData> const InputRecorder::getInputs()
{
	return currentButtonInputs;
}

std::vector<int> const InputRecorder::getKeysToListen()
{
	return keysToListen;
}

void InputRecorder::addKeysToListen(std::vector<int> keysToListen)
{
	this->keysToListen = keysToListen;
}

void InputRecorder::addKeyToListen(int key)
{
	this->keysToListen.push_back(key);
}
