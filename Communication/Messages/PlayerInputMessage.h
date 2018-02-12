#pragma once

#include "../Message.h"
#include "../../Input/Recorders/InputRecorder.h"

class Player;

class PlayerInputMessage : public Message
{
public:
	PlayerInputMessage(const std::string& destinationName, Player* player, ButtonInputData data);
	~PlayerInputMessage();

	//std::string getDataField(std::string name) override;

	Player* player;
	ButtonInputData data;
};

