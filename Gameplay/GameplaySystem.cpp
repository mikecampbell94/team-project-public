#include "GameplaySystem.h"

#include <iostream>
#include "../Communication/Messages/PlayerInputMessage.h"
#include "../Input/Players/Player.h"
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Input/InputUtility.h"
#include "../Resource Management/XMLParser.h"
#include "../Input/Devices/Keyboard.h"

GameplaySystem::GameplaySystem(Database* database)
	: Subsystem("Gameplay")
{
	this->database = database;
 
	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::PLAYER_INPUT, 
		MessageType::COLLISION, MessageType::APPLY_IMPULSE},
		DeliverySystem::getPostman()->getDeliveryPoint("Gameplay"));

	
	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [&gameLogic = gameLogic, &inputBridge = inputBridge,&canjump = canjump](Message* message)
	{
		//gameLogic.notifyMessageActions("PlayerInputMessage", message);
		inputBridge.processPlayerInputMessage(*static_cast<PlayerInputMessage*>(message));
		
		PlayerInputMessage* playerInputMessage = static_cast<PlayerInputMessage*>(message);
		if (playerInputMessage->data.key == KeyboardKeys::KEYBOARD_SPACE)
		{
			canjump = false;			
		}
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::COLLISION, [&gameLogic = gameLogic, &objects = objects, &canjump = canjump](Message* message)
	{
		//CollisionMessage* collisionMessage = static_cast<CollisionMessage*>(message);
		//std::cout << "Obj : " << collisionMessage->objectIdentifier << std::endl;
		//std::cout << "Collider : " << collisionMessage->colliderIdentifier << std::endl;

		gameLogic.notifyMessageActions("CollisionMessage", message);
		
		for (GameObjectLogic& object : objects)
		{
			object.notify("CollisionMessage", message);
		}

		CollisionMessage* collisionmessage = static_cast<CollisionMessage*>(message);
		if ((collisionmessage->colliderIdentifier == "playerBall" || collisionmessage->objectIdentifier == "playerBall"))
		{
			canjump = true;
		}		
	});	
	incomingMessages.addActionToExecuteOnMessage(MessageType::APPLY_IMPULSE, [&canjump = canjump](Message* message)
	{
		if (canjump)
		{
			ApplyImpulseMessage* applyImpulseMessage = static_cast<ApplyImpulseMessage*>(message);
			DeliverySystem::getPostman()->insertMessage(ApplyImpulseMessage("Physics", "playerBall", applyImpulseMessage->impulse));
		}
	});
	
	

}

GameplaySystem::~GameplaySystem()
{
}

void GameplaySystem::updateSubsystem(const float& deltaTime)
{
	gameLogic.executeMessageBasedActions();
	gameLogic.executeTimeBasedActions(deltaTime * 0.001f);
	gameLogic.clearNotifications();

	for each (GameObjectLogic object in objects)
	{
		object.updatelogic(deltaTime * 0.001f);
	}
}

void GameplaySystem::connectPlayerbase(PlayerBase* playerBase)
{
	inputBridge = GameplayInputBridge(playerBase->getPlayers().size());

	for (int i = 0; i < playerBase->getPlayers().size(); i++)//every ionput action map in playersInGame
	{
		inputBridge.addInputActionMapForPlayer(playerBase->getPlayersAction()[i]);
	}
}

void GameplaySystem::compileGameplayScript(std::string levelScript)
{
	objects.clear();

	XMLParser xmlParser;
	xmlParser.loadFile(levelScript);
	gameLogic = GameLogic(&incomingMessages);
	gameLogic.compileParsedXMLIntoScript(xmlParser.parsedXml);
	gameLogic.executeActionsOnStart();

	/*objects.push_back(GameObjectLogic(database, &incomingMessages, "../Data/GameObjectLogic/testObjectLogic.xml"));

	for (GameObjectLogic& object : objects)
	{
		object.compileParsedXMLIntoScript();
	}*/
}

void GameplaySystem::compileGameObjectScripts()
{
	objects.push_back(GameObjectLogic(database, &incomingMessages, "../Data/GameObjectLogic/aiObjectLogic.xml"));
	objects.push_back(GameObjectLogic(database, &incomingMessages, "../Data/GameObjectLogic/playerObjectLogic.xml"));

	for (GameObjectLogic& object : objects)
	{
		object.compileParsedXMLIntoScript();
	}
}

