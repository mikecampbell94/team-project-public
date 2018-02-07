#include "RenderingSystem.h"

#include "../Communication/Message.h"
#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"
#include <queue>

RenderingSystem::RenderingSystem(Window* window, Camera* camera, Vector2 resolution)
	: Subsystem("RenderingSystem")
{
	renderer = std::make_unique<Renderer>(window, camera, resolution);

	std::vector<MessageType> types = { DUMMY_TYPE };

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getMessageBufferByName("RenderingSystem"));
	incomingMessages.addActionToExecuteOnMessage(DUMMY_TYPE, [](Message message)
	{
		std::cout << "Renderer recieved dummy message" << std::endl;
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
