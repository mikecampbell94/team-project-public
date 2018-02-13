#include "GameplaySystem.h"

#include <iostream>
#include "../Communication/Messages/PlayerInputMessage.h"
#include "../Input/Players/Player.h"
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Input/InputUtility.h"
#include "../Resource Management/XMLParser.h"

GameplaySystem::GameplaySystem(const int playersInGame)
	: Subsystem("Gameplay")
{
	InputActionMap actions(0);
	inputParser.loadFile("../Data/Resources/Input/configXML.xml");
	Node* node = inputParser.parsedXml;

	//NEED TO FUCK ABOUT WITH THIS TO ADD ANYTHING OTHER THAN WASD MOVEMENT
	for (int i = 0; i < inputParser.parsedXml->children.size(); i++) {
		actions.attachKeyToAction(InputUtility::getKeyID(inputParser.parsedXml->children[i]->children[0]->value), [node, i](Player* player)
		{
			player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(Vector3(
				stoi(node->children[i]->children[1]->children[0]->children[0]->value),
				stoi(node->children[i]->children[1]->children[0]->children[1]->value),
				stoi(node->children[i]->children[1]->children[0]->children[2]->value))));
		});
	}

	inputBridge = GameplayInputBridge(playersInGame);
	inputBridge.addInputActionMapForPlayer(actions);

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
