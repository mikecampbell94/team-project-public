#include "GameplaySystem.h"

#include <iostream>
#include "../Communication/Messages/PlayerInputMessage.h"
#include "../Input/Player.h"
//#include "../Utilities/Maths/Vector3.h"
//#include "../Utilities/Maths/Matrix4.h"
#include "../Graphics/Scene Management/SceneNode.h"

GameplaySystem::GameplaySystem(const int playersInGame)
	: Subsystem("Gameplay")
{
	InputActionMap actions(0);
	actions.attachKeyToAction(87, [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(0, 0, 1)));
	}); 

	inputBridge = GameplayInputBridge(playersInGame);
	inputBridge.addInputActionMapForPlayer(actions);

	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::PLAYER_INPUT }, 
		DeliverySystem::getPostman()->getDeliveryPoint("Gameplay"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [&inputBridge = inputBridge](Message* message)
	{
		inputBridge.processPlayerInputMessage(*static_cast<PlayerInputMessage*>(message));
	});
}

GameplaySystem::~GameplaySystem()
{
}

void GameplaySystem::updateSubsystem(const float& deltaTime)
{
}
