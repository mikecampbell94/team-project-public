#include "RenderingSystem.h"

#include "../Communication/Message.h"
#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

#include "../Communication/Message.h"

#include <queue>
#include "../../Communication/Messages/PlayerInputMessage.h"

RenderingSystem::RenderingSystem(Window* window, Camera* camera, Vector2 resolution)
	: Subsystem("RenderingSystem")
{
	renderer = std::make_unique<Renderer>(window, camera, resolution);

	std::vector<MessageType> types = { MessageType::DUMMY_TYPE, MessageType::PLAYER_INPUT };

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("RenderingSystem"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::DUMMY_TYPE, [](Message* message)
	{
		std::cout << "Renderer recieved dummy message" << std::endl;
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAYER_INPUT, [](Message* message)
	{
		PlayerInputMessage* playerMessage = static_cast<PlayerInputMessage*>(message);
		std::cout << "Player : " << playerMessage->player->getPlayerID() << std::endl;
		std::cout << "key : " << playerMessage->data.key << std::endl;
		std::cout << "State : " << playerMessage->data.currentState << std::endl;
	});
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
