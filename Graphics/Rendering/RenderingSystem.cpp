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
#include "../../Communication/Messages/ToggleGraphicsModuleMessage.h"
#include "../../Communication/Messages/MoveCameraRelativeToGameObjectMessage.h"
#include "../../Communication/Messages/PreparePaintSurfaceMessage.h"

RenderingSystem::RenderingSystem(Window* window, Camera* camera)
	: Subsystem("RenderingSystem")
{
	this->camera = camera;
	renderer = std::make_unique<Renderer>(window, camera);
}

RenderingSystem::~RenderingSystem()
{
}

void RenderingSystem::initialise(Database* database)
{

	std::vector<MessageType> types = { MessageType::TEXT, MessageType::PLAYER_INPUT, MessageType::RELATIVE_TRANSFORM,
		MessageType::TOGGLE_GRAPHICS_MODULE, MessageType::MOVE_CAMERA_RELATIVE_TO_GAMEOBJECT, MessageType::PREPARE_PAINT_SURFACE};

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

	incomingMessages.addActionToExecuteOnMessage(MessageType::RELATIVE_TRANSFORM, [database = database](Message* message)
	{
		RelativeTransformMessage* translationMessage = static_cast<RelativeTransformMessage*>(message);
		GameObject* gameObject = static_cast<GameObject*>(
			database->getTable("GameObjects")->getResource(translationMessage->resourceName));

		gameObject->getSceneNode()->SetTransform(gameObject->getSceneNode()->GetTransform()
			* translationMessage->transform);

	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::TOGGLE_GRAPHICS_MODULE, [&renderer = renderer](Message* message)
	{
		ToggleGraphicsModuleMessage* moduleMessage = static_cast<ToggleGraphicsModuleMessage*>(message);
		renderer->toggleModule(moduleMessage->moduleName, moduleMessage->enabled);
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::MOVE_CAMERA_RELATIVE_TO_GAMEOBJECT, [&camera = camera, database = database](Message* message)
	{
		MoveCameraRelativeToGameObjectMessage* movementMessage = static_cast<MoveCameraRelativeToGameObjectMessage*>(message);

		GameObject* gameObject = static_cast<GameObject*>(
			database->getTable("GameObjects")->getResource(movementMessage->resourceName));

		camera->setPosition(gameObject->getSceneNode()->GetTransform().getPositionVector() + movementMessage->translation);
		camera->setPitch(movementMessage->pitch);
		camera->setYaw(movementMessage->yaw);
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::PREPARE_PAINT_SURFACE, [database = database, &renderer = renderer](Message* message)
	{
		PreparePaintSurfaceMessage* paintMessage = static_cast<PreparePaintSurfaceMessage*>(message);

		std::vector<GameObject*> surfaceObjects;

		for each (std::string objectIdentifiers in paintMessage->surfaceObjectIdentifiers)
		{
			surfaceObjects.push_back(static_cast<GameObject*>(
				database->getTable("GameObjects")->getResource(objectIdentifiers)));
		}

		static_cast<PaintTrail*>(renderer->getGraphicsModule("PaintTrail"))->preparePaintSurface(surfaceObjects);
	});
}

void RenderingSystem::SetSceneToRender(SceneManager* scene, Database* database)
{
	renderer->initialise(scene, database);
}

void RenderingSystem::updateSubsystem(const float& deltaTime)
{
	renderer->update(deltaTime);
}