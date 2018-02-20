#include "Menu.h"

Menu::Menu(std::string buttonFile, Database* database)
{
	menu = UserInterfaceBuilder::buildButtons(buttonFile, database);

	for (int i = 0; i < menu.size(); ++i)
	{
		allButtons.push_back(&menu[i]);
	}

	selectedButtonIndex = 0;
	HighlightSelectedButton();
}

Menu::~Menu()
{
}

void Menu::HighlightSelectedButton()
{
	slectedButtonDefaultColour = menu[selectedButtonIndex].colour;
	menu[selectedButtonIndex].colour = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Menu::UnhighlightButton()
{
	menu[selectedButtonIndex].colour = slectedButtonDefaultColour;
}

void Menu::ExecuteSelectedButton()
{
	menu[selectedButtonIndex].action();
}

void Menu::moveSelectedDown()
{
	if (selectedButtonIndex < menu.size() - 1)
	{
		UnhighlightButton();
		++selectedButtonIndex;
		HighlightSelectedButton();
	}
}

void Menu::moveSelectedUp()
{
	if (selectedButtonIndex > 0)
	{
		UnhighlightButton();
		--selectedButtonIndex;
		HighlightSelectedButton();
	}
}

std::vector<Button*>* Menu::getAllButtonsInMenu()
{
	return &allButtons;
}
