#pragma once

#include <string>

class TextMessage;

class InputControl
{
public:
	static void registerAsInputUser(std::string inputUserName);
	static void blockOtherInputs(std::string controller);

private:

};

