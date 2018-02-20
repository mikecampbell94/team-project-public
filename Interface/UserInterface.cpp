#include "UserInterface.h"

#include "../../Input/Devices/Mouse.h"
#include "UserInterfaceDisplay.h"

UserInterface::UserInterface(Mouse* mouse, Vector2 resolution, Database* database) : Subsystem("UserInterface")
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

	menu = new Menu("../Data/UserInterface/MainMenu.xml", database);
	UserInterfaceDisplay::provide(menu);
}

UserInterface::~UserInterface()
{
	delete menu;
}

void UserInterface::updateSubsystem(const float& deltaTime)
{
}
