#include "KeyboardRecorder.h"
//TEST PURPOSES
#include <iostream>


void KeyboardRecorder::fillInputs()
{
	for (int key : keysToListen) {
		if (keyboard->keyTriggered(static_cast<KeyboardKeys>(key))) 
		{
			currentInputs.push_back(ButtonInputData(InputType::TRIGGERED,key));
			std::cout << key;
		}
		else if (keyboard->keyHeld(static_cast<KeyboardKeys>(key))) 
		{
			currentInputs.push_back(ButtonInputData(InputType::HELD, key));
			std::cout << key;
		}
	}

}

