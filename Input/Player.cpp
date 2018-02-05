#include "Player.h"

Player::Player(std::string playerID, InputRecorder *recorder)
{
	this->playerID = playerID;
	this->recorder = recorder;
}

Player::~Player()
{

}

void Player::setPlayerID(std::string newID)
{
	playerID = newID;
}

const std::string Player::getPlayerID()
{
	return playerID;
}

void Player::setInputRecorder(InputRecorder* newInputRecorder)
{
	recorder = newInputRecorder;
}

const InputRecorder* Player::getInputRecorder()
{
	return recorder;
}
