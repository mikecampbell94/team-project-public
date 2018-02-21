#include "RenderingSystem.h"

#include "../Communication/Message.h"
#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

#include "../Communication/Message.h"

#include <queue>
#include "../../Communication/Messages/PlayerInputMessage.h"
#include "../../Communication/Messages/RelativeTransformMessage.h"
#include "../Resource Management/Database/Database.h"
#include "../../Gameplay/GameObject.h"
#include "../../Communication/Messages/TextMessage.h"

RenderingSystem::RenderingSystem(Window* window, Camera* camera)
	: Subsystem("RenderingSystem")
{

	renderer = std::make_unique<Renderer>(window, camera);
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::initialise(Database* database)
{

	std::vector<MessageType> types = { MessageType::TEXT, MessageType::PLAYER_INPUT, MessageType::RELATIVE_TRANSFORM };

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
	Initialise();

	incomingMessages.addActionToExecuteOnMessage(MessageType::RELATIVE_TRANSFORM, [database = database](Message* message)
	{
		RelativeTransformMessage* translationMessage = static_cast<RelativeTransformMessage*>(message);
		GameObject* gameObject = static_cast<GameObject*>(
			database->getTable("GameObjects")->getResource(translationMessage->resourceName));

		gameObject->getSceneNode()->SetTransform(gameObject->getSceneNode()->GetTransform()
			* translationMessage->transform);

	});
}

void RenderingSystem::SetSceneToRender(SceneManager* scene)
{
	renderer->initialise(scene);
}

void RenderingSystem::updateSubsystem(const float& deltaTime)
{
	renderer->update(deltaTime);
}

bool RenderingSystem::stob(std::string string)
{
	std::transform(string.begin(), string.end(), string.begin(), ::tolower);
	std::istringstream is(string);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}


//One initialise function
void RenderingSystem::Initialise()
{


}
