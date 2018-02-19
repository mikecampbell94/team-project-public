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

	button = new Button(Vector4(0, 0, 0, 0), Vector2(100, 100), Vector2(100, 100), []() {}, "", nullptr);
}

UserInterface::~UserInterface()
{
	delete button;
}

void UserInterface::updateSubsystem(const float& deltaTime)
{
	Vector2 bottomLeft;
	bottomLeft.x = button->position.x - button->scale.x;
	bottomLeft.y = button->position.y - button->scale.y;

	Vector2 topRight;
	topRight.x = button->position.x + button->scale.x;
	topRight.y = button->position.y + button->scale.y;

	const Vector2 mousePosition = (mouse->getAbsolutePosition()/* * Vector2(2, 2)*/);// -resolution;

	if (mousePosition.x > bottomLeft.x
		&& mousePosition.x < topRight.x
		&& mousePosition.y > bottomLeft.y
		&& mousePosition.y < topRight.y)
	{
		std::cout << "inside" << std::endl;
	}

	std::cout << mousePosition << std::endl;
}
