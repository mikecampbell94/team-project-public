#pragma once

#include <string>
#include <unordered_map>
#include <functional>

struct Button;

typedef std::function<void()> ButtonAction;

class ButtonActionCreator
{
public:
	ButtonActionCreator();
	~ButtonActionCreator();

	ButtonAction createButtonAction(std::string action);

private:
	std::unordered_map<std::string, ButtonAction> actions;
};

