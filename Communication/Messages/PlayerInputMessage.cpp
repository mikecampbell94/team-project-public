#include "PlayerInputMessage.h"

#include "../Input/Players/Player.h"

PlayerInputMessage::PlayerInputMessage(const std::string& destinationName, Player* player, ButtonInputData data)
	: Message(destinationName, PLAYER_INPUT)
{
	this->player = player;
	this->data = data;

	addIntegerInformation("playerID", std::to_string(player->getPlayerID()));
}

PlayerInputMessage::~PlayerInputMessage()
{
}
