#include "InputInterface.h"



InputInterface::InputInterface()
{

}


InputInterface::~InputInterface()
{
}

void InputInterface::updateInputs()
{
	actionsLastFrame = actionsThisFrame;
	actionsThisFrame = 0;
	int currInputCheck = 1;
	while (currInputCheck != maxInputValue) {
		int keyToCheck = controller->getActionKeyBind(currInputCheck);
		if (controller->getKeyState(keyToCheck))  actionsThisFrame |= currInputCheck;
		currInputCheck <<= 1;
	}
	//TODO:: SEND MESSAGES
}

void InputInterface::fireMessages()
{
	//TODO:: DATA DRIVEN 
	if (actionFirstPressed("MOVEUP")) {
		//FIRE MOVEUP MESSAGE
	}
	if (actionFirstPressed("MOVEDOWN")) {
		//FIRE MOVEDOWNMESSAGE
	}
	if (actionFirstPressed("MOVELEFT")) {
		//FIRE MOVELEFTMESSAGE
	}
	if (actionFirstPressed("MOVERIGHT")) {

	}
}
