#pragma once
#include "InputRecorder.h"
#include "Devices\Keyboard.h"
#include "Devices\Mouse.h"


class KeyboardMouseRecorder :
	public InputRecorder
{
public:
	//TEST DATA

	KeyboardMouseRecorder() { this->keysToListen.push_back(65); };
	~KeyboardMouseRecorder() {};

	 void fillInputs() override;

	 void clearInputs() override { };

	void setKeyboard(Keyboard* keyboard) { this->keyboard = keyboard; };

private:
	Keyboard* keyboard;
	Mouse* mouse;
};

