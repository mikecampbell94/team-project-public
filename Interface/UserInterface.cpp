#include "UserInterface.h"

#include "../../Input/Devices/Keyboard.h"
#include "UserInterfaceDisplay.h"

UserInterface::UserInterface(Keyboard* keyboard, Vector2 resolution) : Subsystem("UserInterface")
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

	menu = nullptr;
}

UserInterface::~UserInterface()
{
	delete menu;
}

void UserInterface::initialise(std::string menuFile, Database* database)
{
	if (menu != nullptr)
	{
		delete menu;
	}

	menu = new Menu(menuFile, database);
	UserInterfaceDisplay::provide(menu);
}

void UserInterface::updateSubsystem(const float& deltaTime)
{
	if (keyboard->keyTriggered(KEYBOARD_ESCAPE))
	{
		interfaceDisplaying = !interfaceDisplaying;
		DeliverySystem::getPostman()->insertMessage(ToggleGraphicsModuleMessage("RenderingSystem", "UIModule", interfaceDisplaying));
	}

	if (interfaceDisplaying)
	{
		if (keyboard->keyTriggered(KEYBOARD_DOWN))
		{
			UserInterfaceDisplay::getInterface()->moveSelectedDown();
		}
		else if (keyboard->keyTriggered(KEYBOARD_UP))
		{
			UserInterfaceDisplay::getInterface()->moveSelectedUp();
		}
		else if (keyboard->keyTriggered(KEYBOARD_RIGHT))
		{
			UserInterfaceDisplay::getInterface()->moveSelectedRight();
		}
		else if (keyboard->keyTriggered(KEYBOARD_LEFT))
		{
			UserInterfaceDisplay::getInterface()->moveSelectedLeft();
		}

		if (keyboard->keyTriggered(KEYBOARD_RETURN))
		{
			UserInterfaceDisplay::getInterface()->ExecuteSelectedButton();
		}
	}
}
