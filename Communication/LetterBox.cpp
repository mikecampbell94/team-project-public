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

void LetterBox::insertMessage(TextMessage message)
{
	textMessages.push_back(message);
}

void LetterBox::insertMessage(RelativeTransformMessage message)
{
	sceneNodeTranslationMessages.push_back(message);
}
void LetterBox::insertMessage(PlaySoundMessage message)
{
	playSoundMessages.push_back(message);
}

void LetterBox::insertMessage(StopSoundMessage message)
{
	stopSoundMessages.push_back(message);
}

void LetterBox::insertMessage(ToggleGraphicsModuleMessage message)
{
	graphicsModuleMessages.push_back(message);
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

	for (unsigned int i = 0; i < textMessages.size(); ++i)
	{
		messageStorage.sendMessage(&textMessages[i]);
	}

	for (unsigned int i = 0; i < sceneNodeTranslationMessages.size(); ++i)
	{
		messageStorage.sendMessage(&sceneNodeTranslationMessages[i]);
	}
	
	for (unsigned int i = 0; i < playSoundMessages.size(); ++i)
	{
		messageStorage.sendMessage(&playSoundMessages[i]);
	}

	for (unsigned int i = 0; i < stopSoundMessages.size(); ++i)
	{
		messageStorage.sendMessage(&stopSoundMessages[i]);
	}

	for (unsigned int i = 0; i < graphicsModuleMessages.size(); ++i)
	{
		messageStorage.sendMessage(&graphicsModuleMessages[i]);
	}
}

void LetterBox::clearAllMessages()
{
	messageStorage.clearMessageStorage();

	messages.clear();
	playerInputMessages.clear();
	textMessages.clear();
	sceneNodeTranslationMessages.clear();	
	playSoundMessages.clear();
	stopSoundMessages.clear();
	graphicsModuleMessages.clear();
}
