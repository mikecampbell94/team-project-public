#include "UserInterface.h"

#include "../../Input/Devices/Mouse.h"

UserInterface::UserInterface(Mouse* mouse, Vector2 resolution) : Subsystem("UserInterface")
{
	this->mouse = mouse;
	this->resolution = resolution;

	std::vector<MessageType> types = { MessageType::TEXT };

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("UserInterface"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);
		std::cout << textMessage->text << std::endl;
	});
}

UserInterface::~UserInterface()
{
}

void UserInterface::updateSubsystem(const float& deltaTime)
{
}
