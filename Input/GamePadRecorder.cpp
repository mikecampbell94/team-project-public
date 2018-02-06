#include "GamePadRecorder.h"
#include <iostream>




GamePadRecorder::~GamePadRecorder()
{
}

void GamePadRecorder::fillInputs()
{
	gamepad->Update();
	for (int key : keysToListen) {
		if (gamepad->GetButtonPressed(key)) {
			currentButtonInputs.push_back(ButtonInputData(InputType::TRIGGERED,key));
			std::cout << key << "PRESSED" << "\n";
		}
		if (gamepad->GetButtonDown(key)) {
			currentButtonInputs.push_back(ButtonInputData(InputType::HELD,key));
			std::cout << key << "HELD" << "\n";
		}
		
	}

	
}
