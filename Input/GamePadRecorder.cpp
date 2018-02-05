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
			currentInputs.push_back(KeyState(state::TRIGGERED,key));
			std::cout << key << "PRESSED" << "\n";
		}
		if (gamepad->GetButtonDown(key)) {
			currentInputs.push_back(KeyState(state::HELD,key));
			std::cout << key << "HELD" << "\n";
		}
		
	}

	
}
