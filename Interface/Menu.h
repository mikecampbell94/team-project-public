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
	void UnhighlightButton();
	void ExecuteSelectedButton() override;

	void moveSelectedDown() override;
	void moveSelectedUp() override;

	std::vector<Button*>* getAllButtonsInMenu() override;

private:
	std::vector<Button> menu;
	std::vector<Button*> allButtons;

	Vector4 slectedButtonDefaultColour;
	int selectedButtonIndex;
};

