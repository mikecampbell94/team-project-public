#include "Menu.h"

Menu::Menu(std::string buttonFile, Database* database)
{
	menu = UserInterfaceBuilder::buildButtons(buttonFile, database);

	for (int i = 0; i < menu.size(); ++i)
	{
		allButtons.push_back(&menu[i]);
	}
}

Menu::~Menu()
{
}

void Menu::HighlightSelectedButton()
{
}

void Menu::ExecuteSelectedButton()
{
}

std::vector<Button*>* Menu::getAllButtonsInMenu()
{
	return &allButtons;
}
