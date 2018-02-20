#include "ToggleGraphicsModuleMessage.h"

ToggleGraphicsModuleMessage::ToggleGraphicsModuleMessage(const std::string& destinationName, 
	const std::string& moduleName, const bool enabled)
	: Message(destinationName, TOGGLE_GRAPHICS_MODULE)
{
	this->enabled = enabled;
	this->moduleName = moduleName;
}

ToggleGraphicsModuleMessage::~ToggleGraphicsModuleMessage()
{
}
