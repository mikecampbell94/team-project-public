#include "GamePadRecorder.h"
#include <iostream>

GamePadRecorder::GamePadRecorder(Gamepad * gamePad)
{
	this->gamepad = gamePad;
}

GamePadRecorder::~GamePadRecorder()
{
}

void GamePadRecorder::fillInputs()
{
	gamepad->Update();

	for (int key : keysToListen) 
	{
		if (gamepad->GetButtonPressed(key)) 
		{
			currentButtonInputs.push_back(ButtonInputData(InputType::TRIGGERED,key));
			std::cout << key << "PRESSED" << "\n";
		}

		if (gamepad->GetButtonDown(key)) 
		{
			currentButtonInputs.push_back(ButtonInputData(InputType::HELD,key));
			std::cout << key << "HELD" << "\n";
		}
	}

	currentLinearInputs.push_back(LinearInputData(gamepad->LeftTrigger(),"leftTrigger"));
	currentLinearInputs.push_back(LinearInputData(gamepad->RightTrigger(),"rightTrigger"));
	
	currentPositionalInputs.push_back(PositionalInputData(Vector2(gamepad->LeftStick_X(),gamepad->LeftStick_Y()),"leftStick"));
	currentPositionalInputs.push_back(PositionalInputData(Vector2(gamepad->RightStick_X(), gamepad->RightStick_Y()), "rightStick"));
}
