#pragma once

#include "../Message.h"
#include "../../Input/Recorders/InputRecorder.h"

class TogglePlayerInputKeyMessage : public Message
{
public:
	TogglePlayerInputKeyMessage(const std::string& destinationName, const std::string& playerGameObjectName, int key, bool enabled);
	~TogglePlayerInputKeyMessage();

	static TogglePlayerInputKeyMessage builder(Node* node);

	std::string playerGameObjectName; 
	int key; 
	bool enabled;
};

