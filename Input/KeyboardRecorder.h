#pragma once
#include "InputRecorder.h"
#include "Devices\Keyboard.h"
class KeyboardRecorder :
	public InputRecorder
{
public:
	KeyboardRecorder(playerBase* pb) : InputRecorder(pb) {};
	~KeyboardRecorder() {};

	virtual void fillInputs();

private:
	Keyboard* keyboard;
};

