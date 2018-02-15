#include "GameplaySystem.h"

#include <iostream>
#include "../Communication/Messages/PlayerInputMessage.h"
#include "../Input/Players/Player.h"
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Input/InputUtility.h"
#include "../Resource Management/XMLParser.h"

GameplaySystem::GameplaySystem(const int playersInGame,PlayerBase &playerBase)
	: Subsystem("Gameplay")
{

	inputBridge = GameplayInputBridge(playersInGame);

	for (int i = 0; i < playersInGame; i++)//every ionput action map in playersInGame
	{
		inputBridge.addInputActionMapForPlayer(playerBase.getPlayersAction()[i]);
	}

	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::PLAYER_INPUT }, 
		DeliverySystem::getPostman()->getDeliveryPoint("Gameplay"));

	XMLParser xmlParser;
	xmlParser.loadFile("../Resources/Gameplay/gameplay.xml");
	gameLogic = GameLogic(&incomingMessages);
	gameLogic.compileParsedXMLIntoScript(xmlParser.parsedXml);

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [&gameLogic = gameLogic, &inputBridge = inputBridge](Message* message)
	{
		gameLogic.notifyMessageActions("PlayerInputMessage", message);
		inputBridge.processPlayerInputMessage(*static_cast<PlayerInputMessage*>(message));
	});
}

GameplaySystem::~GameplaySystem()
{
}

void GameplaySystem::updateSubsystem(const float& deltaTime)
{
	gameLogic.executeMessageBasedActions();
	gameLogic.clearNotifications();
}
