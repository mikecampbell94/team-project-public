#pragma once
#include "InputRecorder.h"
#include "Devices\Keyboard.h"
class KeyboardRecorder :
	public InputRecorder
{
public:
	//TEST DATA

	KeyboardRecorder() { this->keysToListen.push_back(65); };
	~KeyboardRecorder() {};

	 void fillInputs() override;

	 void clearInputs() override { };

	void setKeyboard(Keyboard* keyboard) { this->keyboard = keyboard; };

private:
	Keyboard* keyboard;

};

