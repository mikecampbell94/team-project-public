#pragma once

#include "UserInterfaceBuilder.h"
#include "UserInterfaceService.h"

#include <string>
#include <vector>

class Database;

class Menu : public UserInterfaceService
{
public:
	explicit Menu(std::string buttonFile, Database* database);
	~Menu();

	void HighlightSelectedButton();
	void ExecuteSelectedButton();

	std::vector<Button*>* getAllButtonsInMenu() override;

private:
	std::vector<Button> menu;
	std::vector<Button*> allButtons;
};

