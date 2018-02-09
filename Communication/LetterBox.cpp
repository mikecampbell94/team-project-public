#include "LetterBox.h"
#include "DeliverySystem.h"

LetterBox::LetterBox()
{
	messageStorage = MessageStorage();
}

LetterBox::~LetterBox()
{
}

void LetterBox::addDeliveryPoint(const std::string& bufferName)
{
	messageStorage.addMessageBuffer(bufferName);
}

std::queue<Message*>* LetterBox::getDeliveryPoint(const std::string& bufferName)
{
	return messageStorage.getMessageBufferByName(bufferName);
}

void LetterBox::insertMessage(Message message)
{
	messages.push_back(message);
}

void LetterBox::insertMessage(PlayerInputMessage message)
{
	playerInputMessages.push_back(message);
}

void LetterBox::deliverAllMessages()
{
	for (unsigned int i = 0; i < messages.size(); ++i)
	{
		messageStorage.sendMessage(&messages[i]);
	}

	for (unsigned int i = 0; i < playerInputMessages.size(); ++i)
	{
		messageStorage.sendMessage(&playerInputMessages[i]);
	}
}

void LetterBox::clearAllMessages()
{
	messageStorage.clearMessageStorage();

	messages.clear();
	playerInputMessages.clear();
}
