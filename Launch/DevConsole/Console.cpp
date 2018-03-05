#include "Console.h"

#include "../../Input/Devices/Keyboard.h"
#include "../../Input/InputControl.h"
#include "Communication/SendMessageActionBuilder.h"

int consoleKeys[] =
{
	KEYBOARD_0,
	KEYBOARD_1,
	KEYBOARD_2,
	KEYBOARD_3,
	KEYBOARD_4,
	KEYBOARD_5,
	KEYBOARD_6,
	KEYBOARD_7,
	KEYBOARD_8,
	KEYBOARD_9,
	KEYBOARD_Q,
	KEYBOARD_W,
	KEYBOARD_E,
	KEYBOARD_R,
	KEYBOARD_T,
	KEYBOARD_Y,
	KEYBOARD_U,
	KEYBOARD_I,
	KEYBOARD_O,
	KEYBOARD_P,
	KEYBOARD_A,
	KEYBOARD_S,
	KEYBOARD_D,
	KEYBOARD_F,
	KEYBOARD_G,
	KEYBOARD_H,
	KEYBOARD_J,
	KEYBOARD_K,
	KEYBOARD_L,
	KEYBOARD_Z,
	KEYBOARD_X,
	KEYBOARD_C,
	KEYBOARD_V,
	KEYBOARD_B,
	KEYBOARD_N,
	KEYBOARD_M,
	KEYBOARD_SPACE,
	KEYBOARD_COMMA,
	KEYBOARD_PLUS,
	KEYBOARD_MINUS
};

Console::Console(Keyboard* keyboard) : Subsystem("Console")
{
	incomingMessages = MessageProcessor(std::vector<MessageType> {MessageType::TEXT},
		DeliverySystem::getPostman()->getDeliveryPoint("Console"));

	this->keyboard = keyboard;
	enabled = false;
	blocked = false;

	DeliverySystem::getPostman()->insertMessage(TextMessage("InputManager", "RegisterInputUser Console"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [&blocked = blocked](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);

		blocked = InputControl::isBlocked(textMessage->text);
	});

	keyMapping.insert({ KEYBOARD_0, "0" });
	keyMapping.insert({ KEYBOARD_1, "1" });
	keyMapping.insert({ KEYBOARD_2, "2" });
	keyMapping.insert({ KEYBOARD_3, "3" });
	keyMapping.insert({ KEYBOARD_4, "4" });
	keyMapping.insert({ KEYBOARD_5, "5" });
	keyMapping.insert({ KEYBOARD_6, "6" });
	keyMapping.insert({ KEYBOARD_7, "7" });
	keyMapping.insert({ KEYBOARD_8, "8" });
	keyMapping.insert({ KEYBOARD_9, "9" });
	keyMapping.insert({ KEYBOARD_Q, "q" });
	keyMapping.insert({ KEYBOARD_W, "w" });
	keyMapping.insert({ KEYBOARD_E, "e" });
	keyMapping.insert({ KEYBOARD_R, "r" });
	keyMapping.insert({ KEYBOARD_T, "t" });
	keyMapping.insert({ KEYBOARD_Y, "y" });
	keyMapping.insert({ KEYBOARD_U, "u" });
	keyMapping.insert({ KEYBOARD_I, "i" });
	keyMapping.insert({ KEYBOARD_O, "o" });
	keyMapping.insert({ KEYBOARD_P, "p" });
	keyMapping.insert({ KEYBOARD_A, "a" });
	keyMapping.insert({ KEYBOARD_S, "s" });
	keyMapping.insert({ KEYBOARD_D, "d" });
	keyMapping.insert({ KEYBOARD_F, "f" });
	keyMapping.insert({ KEYBOARD_G, "g" });
	keyMapping.insert({ KEYBOARD_H, "h" });
	keyMapping.insert({ KEYBOARD_J, "j" });
	keyMapping.insert({ KEYBOARD_K, "k" });
	keyMapping.insert({ KEYBOARD_L, "l" });
	keyMapping.insert({ KEYBOARD_Z, "z" });
	keyMapping.insert({ KEYBOARD_X, "x" });
	keyMapping.insert({ KEYBOARD_C, "c" });
	keyMapping.insert({ KEYBOARD_V, "v" });
	keyMapping.insert({ KEYBOARD_B, "b" });
	keyMapping.insert({ KEYBOARD_N, "n" });
	keyMapping.insert({ KEYBOARD_M, "m" });
	keyMapping.insert({ KEYBOARD_SPACE, " " });
	keyMapping.insert({ KEYBOARD_COMMA, "," });
	keyMapping.insert({ KEYBOARD_PLUS, "=" });
	keyMapping.insert({ KEYBOARD_MINUS, "-" });
}

Console::~Console()
{
}

void Console::updateSubsystem(const float & deltaTime)
{
	if (keyboard->keyTriggered(KEYBOARD_F7))
	{
		toggleConsoleEnabled();
	}

	if (enabled && !blocked)
	{
		recordKeyPresses();

		if (keyboard->keyTriggered(KEYBOARD_RETURN))
		{
			try
			{
				SendMessageActionBuilder::buildSendMessageAction(input)();
			}
			catch(...)
			{
				input = "error";
			}
		}
	}
}

void Console::toggleConsoleEnabled()
{
	if (enabled)
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("InputManager", "UnblockAll"));
		enabled = false;
	}
	else
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("InputManager", "BlockAllInputs Console"));
		enabled = true;
	}
}

void Console::recordKeyPresses()
{
	++frameCount;

	if(keyboard->keyTriggered(KEYBOARD_CAPITAL))
	{
		capslock =  !capslock;
	}

	if (keyboard->keyDown(KEYBOARD_BACK) && frameCount >= 5)
	{
		frameCount = 0;

		if (input.size() > 0)
		{
			input.pop_back();
		}
	}

	for (int key : consoleKeys)
	{
		if (keyboard->keyStates[key] && !(keyboard->keyStates[key] && keyboard->holdStates[key]))
		{
			if (capslock)
			{
				std::string str = keyMapping.at(key);
				std::transform(str.begin(), str.end(), str.begin(), ::toupper);
				input += str;
			}
			else
			{
				input += keyMapping.at(key);
			}
		}
	}

	std::string displayLine = input;

	for (int i = 0; i < 100 - input.size(); ++i)
	{
		displayLine += " ";
	}

	DeliverySystem::getPostman()->insertMessage(TextMeshMessage("RenderingSystem", displayLine,
		NCLVector3(-620.0f, -320, 0), NCLVector3(12.9f, 12.9f, 12.9f), NCLVector3(0, 1, 0), true, true));
}
