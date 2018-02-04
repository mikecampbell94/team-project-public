#pragma once
#include "InputRecorder.h"
#include "Gamepad.h"

class GamePadRecorder : public InputRecorder
{
public:
	GamePadRecorder() { gamepad = new Gamepad(); this->keysToListen.push_back(1); };
	~GamePadRecorder();

	void  fillInputs() override;
	void clearInputs() override { this->currentInputs.clear(); };


private:
	Gamepad * gamepad;
};

