#include "Player.h"

#include "../Recorders/InputRecorder.h"

Player::Player(int playerID, InputRecorder *recorder/*, SceneNode* playerNode*/)
{
	this->playerID = playerID;
	this->recorder = recorder;
	//node = playerNode;
}

Player::~Player()
{
	delete recorder;
}

void Player::setPlayerID(int newID)
{
	playerID = newID;
}

const int Player::getPlayerID()
{
	return playerID;
}

void Player::setInputRecorder(InputRecorder* newInputRecorder)
{
	recorder = newInputRecorder;
}

 InputRecorder* Player::getInputRecorder()
{
	return recorder;
}

 void Player::setPlayerControls(InputFilter * newControls)
 {
 }

 InputFilter * Player::getInputFilter()
 {
	 return &playerControls;
 }

 void Player::setSceneNode(SceneNode* node)
{
	this->node = node;
}

SceneNode* Player::getSceneNode()
{
	return node;
}
