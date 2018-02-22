#include "LetterBox.h"
#include "DeliverySystem.h"

#include <omp.h>  

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
//#pragma omp critical
	messages.push_back(message);
}

void LetterBox::insertMessage(PlayerInputMessage message)
{
//#pragma omp critical
	playerInputMessages.push_back(message);
}

void LetterBox::insertMessage(TextMessage message)
{
//#pragma omp critical
	textMessages.push_back(message);
}

void LetterBox::insertMessage(RelativeTransformMessage message)
{
//#pragma omp critical
	sceneNodeTranslationMessages.push_back(message);
}
void LetterBox::insertMessage(PlaySoundMessage message)
{
//#pragma omp critical
	playSoundMessages.push_back(message);
}

void LetterBox::insertMessage(StopSoundMessage message)
{
//#pragma omp critical
	stopSoundMessages.push_back(message);
}

void LetterBox::insertMessage(ToggleGraphicsModuleMessage message)
{
//#pragma omp critical
	graphicsModuleMessages.push_back(message);
}

void LetterBox::insertMessage(ApplyForceMessage message)
{
//#pragma omp critical
	applyForceMessages.push_back(message);
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

	for (unsigned int i = 0; i < applyForceMessages.size(); ++i)
	{
		messageStorage.sendMessage(&applyForceMessages[i]);
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
	applyForceMessages.clear();
}
