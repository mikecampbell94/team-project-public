#include "SendMessageActionBuilder.h"

#include "DeliverySystem.h"
#include "Messages/TextMessage.h"
#include "../Resource Management/XMLParser.h"

#include <iterator>
#include <sstream>
#include "Messages/AbsoluteTransformMessage.h"
#include "Messages/MoveGameObjectMessage.h"

std::unordered_map<std::string, Builder>SendMessageActionBuilder::builders 
	= std::unordered_map<std::string, Builder>();

std::unordered_map<std::string, DevConsoleNodeBuilder>SendMessageActionBuilder::devConsoleBuilder
	= std::unordered_map<std::string, DevConsoleNodeBuilder>();

void SendMessageActionBuilder::initialiseBuilders()
{
	initialiseNodeBuilders();
	initialiseDevConsoleBuilders();
}

Executable SendMessageActionBuilder::buildSendMessageAction(Node* node)
{
	return builders.at(node->name)(node);
}

Executable SendMessageActionBuilder::buildSendMessageAction(std::string devConsoleLine)
{
	istringstream iss(devConsoleLine);
	vector<string> tokens{ istream_iterator<string>{iss},
		istream_iterator<string>{} };

	//std::string action = tokens[0];
	//std::transform(action.begin(), action.end(), action.begin(), ::toupper);

	return devConsoleBuilder.at(tokens[0])(tokens);
}

void SendMessageActionBuilder::initialiseNodeBuilders()
{
	builders.insert({ "TEXT" , [](Node* node)
	{
		Node* destination = node->children[0];
		Node* data = node->children[1];

		return[destination = destination->value, text = data->value]()
		{
			DeliverySystem::getPostman()->insertMessage(TextMessage(destination, text));
		};
	} });

	builders.insert({ "RELATIVE_TRANSFORM" , [](Node* node)
	{
		RelativeTransformMessage message = RelativeTransformMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	builders.insert({ "ABSOLUTE_TRANSFORM" , [](Node* node)
	{
		AbsoluteTransformMessage message = AbsoluteTransformMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

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

	builders.insert({ "MOVE_GAMEOBJECT" , [](Node* node)
	{
		MoveGameObjectMessage message = MoveGameObjectMessage::builder(node);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });
}

void SendMessageActionBuilder::initialiseDevConsoleBuilders()
{
	devConsoleBuilder.insert({ "text" , [](std::vector<std::string> line)
	{
		std::string destination = line[1];
		std::string data = line[2];

		return[destination = destination, text = data]()
		{
			DeliverySystem::getPostman()->insertMessage(TextMessage(destination, text));
		};
	} });

	devConsoleBuilder.insert({ "relativetransform" , [](std::vector<std::string> line)
	{
		RelativeTransformMessage message = RelativeTransformMessage::tokensToMessage(line);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	devConsoleBuilder.insert({ "applyforce" , [](std::vector<std::string> line)
	{
		ApplyForceMessage message = ApplyForceMessage::tokensToMessage(line);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	devConsoleBuilder.insert({ "applyimpulse" , [](std::vector<std::string> line)
	{
		ApplyImpulseMessage message = ApplyImpulseMessage::tokensToMessage(line);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	devConsoleBuilder.insert({ "addscoreholder" , [](std::vector<std::string> line)
	{
		AddScoreHolderMessage message = AddScoreHolderMessage::tokensToMessage(line);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });

	devConsoleBuilder.insert({ "movegameobject" , [](std::vector<std::string> line)
	{
		MoveGameObjectMessage message = MoveGameObjectMessage::tokensToMessage(line);

		return [message = message]()
		{
			DeliverySystem::getPostman()->insertMessage(message);
		};
	} });
}
