#pragma once
#include "BaseController.h"
class InputInterface
{
public:
	InputInterface();
	~InputInterface();

	//use bitshifting to create actions this frame
	void updateInputs();

	//message system will determine relevant messages to fire based on actions
	void fireMessages();


	inline bool const actionFirstPressed(int action) { return(actionsThisFrame & action) == action; };
	inline bool const actionFirstPressed(std::string action) { return(actionsThisFrame & actions.at(action)) == actions.at(action); };

	inline bool const actionHeld(int action) { return(actionsLastFrame & actionsThisFrame & action) == action; };
	inline bool const actionHeld(std::string action) { return(actionsLastFrame & actionsThisFrame & actions.at(action)) == actions.at(action); };


	inline bool const actionReleased(int action) {}



protected:
	int maxInputValue;
	int actionsThisFrame;
	int actionsLastFrame;

	//polymorphism to determine whether controller/keyboard at run time
	//TODO:: change for multiple controllers
	BaseController* controller;

	
	std::unordered_map<std::string, int> actions = { {"MOVEUP",1},{"MOVEDOWN",2},{"MOVELEFT",4},{"MOVERIGHT",8} };
};

