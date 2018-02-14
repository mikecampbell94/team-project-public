#include "RenderingSystem.h"

#include "../Communication/Message.h"
#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

#include "../Communication/Message.h"

#include <queue>
#include "../../Communication/Messages/PlayerInputMessage.h"
#include "../../Communication/Messages/SceneNodeTranslationMessage.h"
#include "../Resource Management/Database/Database.h"
#include "../../Gameplay/GameObject.h"
#include "../../Communication/Messages/TextMessage.h"

RenderingSystem::RenderingSystem(Database* database, Window* window, Camera* camera, Vector2 resolution)
	: Subsystem("RenderingSystem")
{
	renderer = std::make_unique<Renderer>(window, camera, resolution);

	std::vector<MessageType> types = { MessageType::TEXT, MessageType::PLAYER_INPUT, MessageType::TRANSLATE_SCENE_NODE };

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("RenderingSystem"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);
		std::cout << textMessage->text << std::endl;
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [](Message* message)
	{
		PlayerInputMessage* playerMessage = static_cast<PlayerInputMessage*>(message);
		std::cout << "Player : " << playerMessage->player->getPlayerID() << std::endl;
		std::cout << "key : " << playerMessage->data.key << std::endl;
		std::cout << "State : " << playerMessage->data.currentState << std::endl;
	});

	//incomingMessages.addActionToExecuteOnMessage(MessageType::TRANSLATE_SCENE_NODE, [database](Message* message)
	//{
	//	SceneNodeTranslationMessage* translationMessage = static_cast<SceneNodeTranslationMessage*>(message);
	//	GameObject* gameObject = static_cast<GameObject*>(
	//		database->getTable("GameObjects")->getAllResources()->getResource(translationMessage->resourceName));
	//	gameObject->

	//});
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::SetSceneToRender(SceneManager* scene)
{
	renderer->initialise(scene);
}

void RenderingSystem::updateSubsystem(const float& deltaTime)
{
	renderer->update(deltaTime);
}
