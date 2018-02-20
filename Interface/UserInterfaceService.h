#pragma once

#include "../Graphics/Graphics Pipeline/PipelineConfiguration.h"

#include <vector>

class UserInterfaceService
{
public:
	virtual std::vector<Button*>* getAllButtonsInMenu() = 0;
};

