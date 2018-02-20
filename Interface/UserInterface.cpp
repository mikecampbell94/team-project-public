#include "UserInterface.h"

#include "../../Input/Devices/Keyboard.h"
#include "UserInterfaceDisplay.h"

UserInterface::UserInterface(Keyboard* keyboard, Vector2 resolution, Database* database) : Subsystem("UserInterface")
{
	this->keyboard = keyboard;
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
	if (keyboard->keyTriggered(KEYBOARD_DOWN))
	{
		UserInterfaceDisplay::getInterface()->moveSelectedDown();
	}
	else if (keyboard->keyTriggered(KEYBOARD_UP))
	{
		UserInterfaceDisplay::getInterface()->moveSelectedUp();
	}

	if (keyboard->keyTriggered(KEYBOARD_RETURN))
	{
		UserInterfaceDisplay::getInterface()->ExecuteSelectedButton();
	}
}
