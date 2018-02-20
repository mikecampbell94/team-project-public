#pragma once

#include "../Message.h"

class ToggleGraphicsModuleMessage : public Message
{
public:
	ToggleGraphicsModuleMessage(const std::string& destinationName, const std::string& moduleName, const bool enabled);
	~ToggleGraphicsModuleMessage();

	std::string moduleName;
	bool enabled;
};

