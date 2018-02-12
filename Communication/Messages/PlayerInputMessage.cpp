#include "PlayerInputMessage.h"

#include "../Input/Players/Player.h"

PlayerInputMessage::PlayerInputMessage(const std::string& destinationName, Player* player, ButtonInputData data)
	: Message(destinationName, PLAYER_INPUT)
{
	this->player = player;
	this->data = data;

	//integerInformation.insert({ "playerID", std::to_string(player->getPlayerID()) });
	addIntegerInformation("playerID", "0");
}

PlayerInputMessage::~PlayerInputMessage()
{
}

//std::string PlayerInputMessage::getDataField(std::string name)
//{
//	if (name == "destination")
//	{
//		return destination;
//	}
//	else if (name == "type")
//	{
//		return messageTypeData.at(type);
//	}
//	else if (name == "playerID")
//	{
//		return std::to_string(player->getPlayerID());
//	}
//}
