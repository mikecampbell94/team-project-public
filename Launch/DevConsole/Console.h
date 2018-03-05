#pragma once

#include "../Systems/Subsystem.h"

class Keyboard;

class Console : public Subsystem
{
public:
	Console(Keyboard* keyboard);
	~Console();

	void updateSubsystem(const float& deltaTime) override;

private:
	void toggleConsoleEnabled();
	void recordKeyPresses();

	Keyboard* keyboard;

	std::string input;
	bool enabled;
	bool blocked;
	bool capslock = false;
	int frameCount = 0;

	std::unordered_map<int, std::string> keyMapping;
};

