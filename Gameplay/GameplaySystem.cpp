#include "GameplaySystem.h"

#include <iostream>
#include "../Communication/Messages/PlayerInputMessage.h"
#include "../Input/Player.h"
#include "../Graphics/Scene Management/SceneNode.h"
#include "Input/InputUtility.h"

GameplaySystem::GameplaySystem(const int playersInGame)
	: Subsystem("Gameplay")
{
	InputActionMap actions(0);
	actions.attachKeyToAction(InputUtility::getKeyID("KEYBOARD_W"), [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(0, 0, 1)));
	}); 

	actions.attachKeyToAction(InputUtility::getKeyID("KEYBOARD_A"), [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(-1, 0, 0)));
	});

	actions.attachKeyToAction(InputUtility::getKeyID("KEYBOARD_S"), [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(0, 0, -1)));
	});

	actions.attachKeyToAction(InputUtility::getKeyID("KEYBOARD_D"), [](Player* player)
	{
		player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(1, 0, 0)));
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
