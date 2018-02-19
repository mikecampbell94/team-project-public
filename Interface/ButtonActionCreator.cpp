#include "ButtonActionCreator.h"

#include "UserInterfaceBuilder.h"

ButtonActionCreator::ButtonActionCreator()
{
	actions.insert({"PrintText", []()
	{
		std::cout << "Button pressed" << std::endl;
	} });
}

ButtonActionCreator::~ButtonActionCreator()
{
}

ButtonAction ButtonActionCreator::createButtonAction(std::string action)
{
	return actions.at(action);
}