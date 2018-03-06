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

	void traverseInputHistory();
	void deleteLastCharacter();
	void readKeyboardInputs();
	void displayText();

	Keyboard* keyboard;

	std::string input;
	bool enabled;
	bool blocked;
	bool capslock = false;
	int frameCount = 0;
	int previousInputIndexOffset = 0;

	std::deque<std::string> previousInputs;
	std::unordered_map<int, std::string> keyMapping;
};

