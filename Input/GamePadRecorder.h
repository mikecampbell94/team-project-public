#pragma once
#include "InputRecorder.h"
#include "Gamepad.h"

class GamePadRecorder : public InputRecorder
{
public:
	GamePadRecorder(Gamepad* gamePad);
	~GamePadRecorder();

	void  fillInputs() override;


private:
	Gamepad * gamepad;
};

