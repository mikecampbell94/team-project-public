#include "ButtonActionCreator.h"

#include "UserInterfaceBuilder.h"
#include "../Communication/DeliverySystem.h"

ButtonActionCreator::ButtonActionCreator()
{
	actions.insert({"PrintText", []()
	{
		std::cout << "Button pressed" << std::endl;
	} });

	actions.insert({ "Start", []()
	{
		std::cout << "Start" << std::endl;
	} });

	actions.insert({ "Quit", []()
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "Quit"));
	} });
}

ButtonActionCreator::~ButtonActionCreator()
{
}

ButtonAction ButtonActionCreator::createButtonAction(std::string action)
{
	return actions.at(action);
}