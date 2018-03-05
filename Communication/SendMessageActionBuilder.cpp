#include "SendMessageActionBuilder.h"

#include "DeliverySystem.h"
#include "Messages/TextMessage.h"
#include "../Resource Management/XMLParser.h"

std::unordered_map<std::string, Builder>SendMessageActionBuilder::builders 
	= std::unordered_map<std::string, Builder>();

void SendMessageActionBuilder::initialiseBuilders()
{
	builders.insert({ "TEXT" , [](Node* node)
	{
		Node* destination = node->children[0];
		Node* data = node->children[1];

		return[destination = destination->value, text = data->value]()
		{
			DeliverySystem::getPostman()->insertMessage(TextMessage(destination, text));
		};
	}});

	builders.insert({ "RELATIVE_TRANSFORM" , [](Node* node)
	{
		RelativeTransformMessage message = RelativeTransformMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	}});

	builders.insert({ "MOVE_CAMERA_RELATIVE_TO_GAMEOBJECT" , [](Node* node)
	{
		MoveCameraRelativeToGameObjectMessage message = MoveCameraRelativeToGameObjectMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	builders.insert({ "TOGGLE_GRAPHICS_MODULE" , [](Node* node)
	{
		ToggleGraphicsModuleMessage message = ToggleGraphicsModuleMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	builders.insert({ "PREPARE_PAINT_SURFACE" , [](Node* node)
	{
		PreparePaintSurfaceMessage message = PreparePaintSurfaceMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	builders.insert({ "PAINT_TRAIL_FOR_GAMEOBJECT" , [](Node* node)
	{
		PaintTrailForGameObjectMessage message = PaintTrailForGameObjectMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	builders.insert({ "APPLY_IMPULSE" , [](Node* node)
	{
		ApplyImpulseMessage message = ApplyImpulseMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	builders.insert({ "APPLY_FORCE" , [](Node* node)
	{
		ApplyForceMessage message = ApplyForceMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	builders.insert({ "ADD_SCORE_HOLDER" , [](Node* node)
	{
		AddScoreHolderMessage message = AddScoreHolderMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });
}

Executable SendMessageActionBuilder::buildSendMessageAction(Node* node)
{
	return builders.at(node->name)(node);
}
