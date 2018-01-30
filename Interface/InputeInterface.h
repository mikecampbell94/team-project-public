#pragma once
#include "BaseController.h"
class InputeInterface
{
public:
	InputeInterface();
	~InputeInterface();

	//use bitshifting to create actions this frame
	void updateInputs();

protected:
	int maxInputValue;
	int actionsThisFrame;
	int actionsLastFrame;

	//polymorphism to determine whether controller/keyboard at run time
	BaseController* Bc;
};

