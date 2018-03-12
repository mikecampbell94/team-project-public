#include "GameplaySystem.h"

#include <iostream>
#include "../Communication/Messages/PlayerInputMessage.h"
#include "../Input/Players/Player.h"
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Input/InputUtility.h"
#include "../Resource Management/XMLParser.h"
#include "../Utilities/GameTimer.h"
#include "../Input/Devices/Keyboard.h"
#include "../Communication/SendMessageActionBuilder.h"

GameplaySystem::GameplaySystem(Database* database)
	: Subsystem("Gameplay")
{
	this->database = database;
 
	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::PLAYER_INPUT, MessageType::COLLISION, MessageType::TEXT },
		DeliverySystem::getPostman()->getDeliveryPoint("Gameplay"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [this, database = database](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);

		istringstream iss(textMessage->text);
		vector<string> tokens{ istream_iterator<string>{iss},
			std::istream_iterator<string>{} };

		if (tokens[0] == "addgameobjectlogic")
		{
			objects.push_back(GameObjectLogic(database, &incomingMessages, tokens[1]));
			objects[objects.size() - 1].compileParsedXMLIntoScript();
		}
		else if (tokens[0] == "removegameobjectlogic")
		{
			gameObjectLogicRemoveBuffer.push_back(tokens[1]);
		}
		else if (tokens[0] == "setgameplaylogic")
		{
			compileGameplayScript(tokens[1]);
		}
		else if (tokens[0] == "setmaxtime")
		{
			if(tokens[1] == "true")
			{
				gameLogic.isTimed = true;
				gameLogic.maxTime = stof(tokens[2]);
			}
			else
			{
				gameLogic.isTimed = false;
			}
		}
		
	});
	
	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [&gameLogic = gameLogic, &inputBridge = inputBridge, &objects = objects](Message* message)
	{
		inputBridge.processPlayerInputMessage(*static_cast<PlayerInputMessage*>(message));

		for (GameObjectLogic& object : objects)
		{
			object.notify("InputMessage", message);
		}
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::COLLISION, [&gameLogic = gameLogic, &objects = objects](Message* message)
	{
		gameLogic.notifyMessageActions("CollisionMessage", message);

		CollisionMessage* collisionmessage = static_cast<CollisionMessage*>(message);
		
		for (GameObjectLogic& object : objects)
		{
			object.notify("CollisionMessage", message);
		}

	});

	timer->addChildTimer("Level Logic");
	timer->addChildTimer("Object Logic");
}

GameplaySystem::~GameplaySystem()
{
}

void GameplaySystem::updateSubsystem(const float& deltaTime)
{
	if (gameLogic.isTimed) 
	{
		if (gameLogic.elapsedTime < gameLogic.maxTime)
		{
			timer->beginTimedSection();

			timer->beginChildTimedSection("Level Logic");
			gameLogic.executeMessageBasedActions();
			gameLogic.executeTimeBasedActions(deltaTime * 0.001f);
			gameLogic.clearNotifications();
			timer->endChildTimedSection("Level Logic");

			timer->beginChildTimedSection("Object Logic");
			for (GameObjectLogic& object : objects)
			{
				object.updatelogic(deltaTime * 0.001f);
			}
			timer->endChildTimedSection("Object Logic");

			timer->endTimedSection();

			gameLogic.elapsedTime += (deltaTime * 0.001f);

			DeliverySystem::getPostman()->insertMessage(TextMeshMessage("RenderingSystem", std::to_string((int)round(gameLogic.maxTime - gameLogic.elapsedTime)),
				NCLVector3(-75, 310, 0), NCLVector3(30, 30, 30), NCLVector3(1, 0, 0), true, true));
		}
		else if(!levelFinished)
		{
			levelFinished = true;
			DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "deltatime disable"));
			DeliverySystem::getPostman()->insertMessage(TextMessage("UserInterface", "Toggle"));
		}
		else
		{
			//send messages
			DeliverySystem::getPostman()->insertMessage(TextMeshMessage("RenderingSystem", "GAME OVER!",
				NCLVector3(-50, -50, 0), NCLVector3(50, 50, 50), NCLVector3(1, 0, 0), true, true));
		}
	}
	else
	{
		timer->beginTimedSection();

		timer->beginChildTimedSection("Level Logic");
		gameLogic.executeMessageBasedActions();
		gameLogic.executeTimeBasedActions(deltaTime * 0.001f);
		gameLogic.clearNotifications();
		timer->endChildTimedSection("Level Logic");

		timer->beginChildTimedSection("Object Logic");
		for (GameObjectLogic& object : objects)
		{
			object.updatelogic(deltaTime * 0.001f);
		}
		timer->endChildTimedSection("Object Logic");

		timer->endTimedSection();
	}

	for (std::string gameObjectLogicToRemove : gameObjectLogicRemoveBuffer)
	{
		for (int i = 0; i < objects.size(); ++i)
		{
			if (objects[i].getScriptFile() == gameObjectLogicToRemove)
			{
				objects.erase(objects.begin() + i);
				break;
			}
		}
	}

	gameObjectLogicRemoveBuffer.clear();
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
	ActionBuilder::setExecutableBuilder([](Node* node)
	{
		return SendMessageActionBuilder::buildSendMessageAction(node);
	});

	gameplayScript = levelScript;
	gameLogic = GameLogic(&incomingMessages);
	gameLogic.compileScript(levelScript);
	gameLogic.executeActionsOnStart();
}

void GameplaySystem::setDefaultGameplayScript()
{
	gameplayScript = "";
	gameLogic = GameLogic(&incomingMessages);
}

void GameplaySystem::addGameObjectScript(std::string scriptFile)
{
	objects.push_back(GameObjectLogic(database, &incomingMessages, scriptFile));
}

void GameplaySystem::deleteGameObjectScripts()
{
	objects.clear();
}

void GameplaySystem::compileGameObjectScripts()
{
	for (GameObjectLogic& object : objects)
	{
		object.compileParsedXMLIntoScript();
	}
}

