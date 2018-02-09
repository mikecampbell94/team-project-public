#include "GameplaySystem.h"

#include <iostream>
#include "../Communication/Messages/PlayerInputMessage.h"

GameplaySystem::GameplaySystem() 
	: Subsystem("Gameplay")
{
	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::PLAYER_INPUT }, 
		DeliverySystem::getPostman()->getDeliveryPoint("Gameplay"));

	//incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [](Message* message)
	//{
	//	PlayerInputMessage* playerMessage = static_cast<PlayerInputMessage*>(message);
	//	std::cout << "Player : " << playerMessage->player->getPlayerID() << std::endl;
	//	std::cout << "key : " << playerMessage->data.key << std::endl;
	//	std::cout << "State : " << playerMessage->data.currentState << std::endl;
	//});
}


GameplaySystem::~GameplaySystem()
{
}

void GameplaySystem::updateSubsystem(const float& deltaTime)
{
}
