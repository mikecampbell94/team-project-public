#include "GameLoop.h"
#include "../../Input/InputManager.h"


#include "../../Resource Management/XMLParser.h"
#include "../../Resource Management/Level.h"
#include "../../Input/Recorders/KeyboardMouseRecorder.h"
#include <iostream>
#include "Communication/LetterBox.h"
#include "../../Gameplay/GameObject.h"
#include "../../Input/Recorders/KeyboardMouseRecorder.h"
#include "Communication/Messages/PlaySoundMessage.h"
#include "../Startup.h"
#include <iterator>

GameLoop::GameLoop(System* gameSystem, Database* database, Startup* startup)
{
	engine = gameSystem;
	this->database = database;
	this->camera = camera;

	DeliverySystem::getPostman()->addDeliveryPoint("GameLoop");
	incomingMessages = MessageProcessor(std::vector<MessageType>{ MessageType::TEXT},
		DeliverySystem::getPostman()->getDeliveryPoint("GameLoop"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [startup = startup, &quit = quit,
		&deltaTimeMultiplier = deltaTimeMultiplier, &engine = engine, camera = camera](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);

		istringstream iss(textMessage->text);
		vector<string> tokens{ istream_iterator<string>{iss},
			std::istream_iterator<string>{} };

		if (tokens[0] == "Quit")
		{
			quit = true;
		}
		else if (tokens[0] == "Start")
		{
			DeliverySystem::getPostman()->cancelOutgoingMessages();
			startup->switchLevel();
			deltaTimeMultiplier = 1.0f;
			//engine->processMessagesForAllSubsystems();
			//DeliverySystem::getPostman()->clearAllMessages();

			if (tokens[1] == "True")
			{
				startup->beginOnlineLobby();
				startup->loadLevel(tokens[2], true);
			}
			else
			{
				startup->loadLevel(tokens[2], false);
			}
		}
		else if (tokens[0] == "deltatime")
		{
			if (tokens[1] == "enable")
			{
				deltaTimeMultiplier = 1.0f;
			}
			else if (tokens[1] == "disable")
			{
				deltaTimeMultiplier = 0.0f;
			}
		}
	});
}

GameLoop::~GameLoop()
{
}

void GameLoop::executeGameLoop()
{
	camera->setPitch(24.0f);
	camera->setYaw(-133.0f);

	//MOVE AUDIO FROM GAME LOOP TO SOMEWHERE ELSE LIKE GAMEPLAY
	DeliverySystem::getPostman()->insertMessage(PlayMovingSoundMessage("AudioSystem", camera->getPersistentPosition(),
		SOUNDPRIORITY_HIGH, 1.0f, 1.0f, 1.0f, true, "mirrorsedge", "BackgroundMusic"));

	while (window->updateWindow() && !quit)
	{
		float deltaTime = loopTimer->getTimeSinceLastRetrieval() * deltaTimeMultiplier;

		engine->updateNextSystemFrame(deltaTime);
		updateGameObjects();

		DeliverySystem::getPostman()->deliverAllMessages();

		engine->processMessagesForAllSubsystems();
		incomingMessages.processMessagesInBuffer();

		DeliverySystem::getPostman()->clearAllMessages();

		//pitch -= (window->getMouse()->getRelativePosition().y);
		//yaw -= (window->getMouse()->getRelativePosition().x);

		if (window->getKeyboard()->keyDown(KEYBOARD_W)) {
			camera->setPosition(camera->getPosition() +
				NCLMatrix4::rotation(yaw, NCLVector3(0, 1, 0)) * NCLVector3(0, 0, -1) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_S)) {
			camera->setPosition(camera->getPosition() +
				NCLMatrix4::rotation(yaw, NCLVector3(0, 1, 0)) * NCLVector3(0, 0, 1) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_A)) {
			camera->setPosition(camera->getPosition() +
				NCLMatrix4::rotation(yaw, NCLVector3(0, 1, 0)) *  NCLVector3(-1, 0, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_D)) {
			camera->setPosition(camera->getPosition() +
				NCLMatrix4::rotation(yaw, NCLVector3(0, 1, 0)) *  NCLVector3(1, 0, 0) * 1);
		}

		if (window->getKeyboard()->keyDown(KEYBOARD_SPACE)) {
			camera->setPosition(camera->getPosition() + NCLVector3(0, 1, 0) * 1);
		}
		
		//camera->setPitch(pitch);
		//camera->setYaw(yaw);
	}

}

void GameLoop::updateGameObjects()
{
	auto gameObjectResources = database->getTable("GameObjects")->getAllResources()->getResourceBuffer();
	for (auto gameObjectIterator = gameObjectResources.begin(); gameObjectIterator != gameObjectResources.end(); ++gameObjectIterator)
	{
		GameObject* gObj = static_cast<GameObject*>((*gameObjectIterator).second);
		if (gObj->getPhysicsNode() != nullptr)
		{
			gObj->updatePosition();
		}
		//DeliverySystem::getPostman()->insertMessage(TextMeshMessage("RenderingSystem", "thing",
		//	gObj->getSceneNode()->GetWorldTransform().getPositionVector(), Vector3(10, 10, 1), false));
	}
}
