#include "ButtonActionCreator.h"

#include "UserInterfaceBuilder.h"
#include "../Communication/DeliverySystem.h"

ButtonActionCreator::ButtonActionCreator()
{
	actions.insert({ "None", []()
	{
	} });

	actions.insert({"PrintText", []()
	{
		std::cout << "Button pressed" << std::endl;
	} });

	actions.insert({ "Start", []()
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "Start"));
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "UIModule", false));
	} });

	actions.insert({ "Quit", []()
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "Quit"));
	} });

	actions.insert({ "DisableSSAO", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "SSAO", false));
	} });

	actions.insert({ "EnableSSAO", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "SSAO", true));
	} });

	actions.insert({ "EnableSkybox", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "Skybox", true));
	} });

	actions.insert({ "DisableSkybox", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "Skybox", false));
	} });

	actions.insert({ "EnableShadows", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "Shadows", true));
	} });

	actions.insert({ "DisableShadows", []()
	{
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "Shadows", false));
	} });
}

ButtonActionCreator::~ButtonActionCreator()
{
}

ButtonAction ButtonActionCreator::createButtonAction(std::string action)
{
	return actions.at(action);
}