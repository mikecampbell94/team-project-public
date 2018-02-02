#pragma once
#include "InputRecorder.h"
#include "Devices\Keyboard.h"
class KeyboardRecorder :
	public InputRecorder
{
public:
	//TEST DATA

	KeyboardRecorder(playerBase* pb) : InputRecorder(pb) { this->keysToListen.push_back(65); };
	~KeyboardRecorder() {};

	virtual void fillInputs();

	virtual void clearInputs() { this->currentInputs.clear(); };

	void setKeyboard(Keyboard* keyboard) { this->keyboard = keyboard; };

private:
	Keyboard* keyboard;
};

