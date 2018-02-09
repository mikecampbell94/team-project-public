#include "PlayerInputMessage.h"

#include "../Input/Players/Player.h"

PlayerInputMessage::PlayerInputMessage(const std::string& destinationName, Player* player, ButtonInputData data)
	: Message(destinationName, PLAYER_INPUT)
{
	this->player = player;
	this->data = data;
}

PlayerInputMessage::~PlayerInputMessage()
{
}
