#pragma once

#include "../Systems/Subsystem.h"

class Keyboard;

class Console : public Subsystem
{
public:
	Console(Keyboard* keyboard);
	~Console();

	void updateSubsystem(const float& deltaTime);

private:
	Keyboard* keyboard;

	std::string input;
	bool enabled;

	std::unordered_map<int, std::string> keyMapping;
};

