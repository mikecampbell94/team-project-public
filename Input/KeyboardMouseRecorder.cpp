#include "KeyboardMouseRecorder.h"
//TEST PURPOSES
#include <iostream>


void KeyboardMouseRecorder::fillInputs()
{
	for (int key : keysToListen) {
		if (keyboard->keyTriggered(static_cast<KeyboardKeys>(key))) 
		{
			currentButtonInputs.push_back(ButtonInputData(InputType::TRIGGERED,key));
			std::cout << key;
		}
		else if (keyboard->keyHeld(static_cast<KeyboardKeys>(key))) 
		{
			currentButtonInputs.push_back(ButtonInputData(InputType::HELD, key));
			std::cout << key;
		}
	}

	currentPositionalInputs.push_back(PositionalInputData(mouse->getRelativePosition(),"relativePosition"));
	currentPositionalInputs.push_back(PositionalInputData(mouse->getAbsolutePosition(),"absolutePosition"));

}

