#include "InputRecorder.h"

std::vector<ButtonInputData> const InputRecorder::getInputs()
{
	return currentInputs;
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
