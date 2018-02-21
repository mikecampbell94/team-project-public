#include "OutgoingMessageBuffer.h"
#include "DeliverySystem.h"

OutgoingMessageBuffer::OutgoingMessageBuffer()
{
}

OutgoingMessageBuffer::~OutgoingMessageBuffer()
{
}
void OutgoingMessageBuffer::insertMessage(Message message)
{
	messages.push_back(message);
}

void OutgoingMessageBuffer::insertMessage(PlayerInputMessage message)
{
	playerInputMessages.push_back(message);
}

void OutgoingMessageBuffer::insertMessage(TextMessage message)
{
	textMessages.push_back(message);
}

void OutgoingMessageBuffer::insertMessage(RelativeTransformMessage message)
{
	sceneNodeTranslationMessages.push_back(message);
}
void OutgoingMessageBuffer::insertMessage(PlaySoundMessage message)
{
	playSoundMessages.push_back(message);
}

void OutgoingMessageBuffer::insertMessage(StopSoundMessage message)
{
	stopSoundMessages.push_back(message);
}

void OutgoingMessageBuffer::insertMessage(ToggleGraphicsModuleMessage message)
{
	graphicsModuleMessages.push_back(message);
}

void OutgoingMessageBuffer::insertMessage(ApplyForceMessage message)
{
	applyForceMessages.push_back(message);
}

void OutgoingMessageBuffer::deliverAllMessages()
{
	for (unsigned int i = 0; i < messages.size(); ++i)
	{
		DeliverySystem::getPostman()->insertMessage(messages[i]);
	}

	for (unsigned int i = 0; i < playerInputMessages.size(); ++i)
	{
		DeliverySystem::getPostman()->insertMessage(playerInputMessages[i]);
	}

	for (unsigned int i = 0; i < textMessages.size(); ++i)
	{
		DeliverySystem::getPostman()->insertMessage(textMessages[i]);
	}

	for (unsigned int i = 0; i < sceneNodeTranslationMessages.size(); ++i)
	{
		DeliverySystem::getPostman()->insertMessage(sceneNodeTranslationMessages[i]);
	}

	for (unsigned int i = 0; i < playSoundMessages.size(); ++i)
	{
		DeliverySystem::getPostman()->insertMessage(playSoundMessages[i]);
	}

	for (unsigned int i = 0; i < stopSoundMessages.size(); ++i)
	{
		DeliverySystem::getPostman()->insertMessage(stopSoundMessages[i]);
	}

	for (unsigned int i = 0; i < graphicsModuleMessages.size(); ++i)
	{
		DeliverySystem::getPostman()->insertMessage(graphicsModuleMessages[i]);
	}

	for (unsigned int i = 0; i < applyForceMessages.size(); ++i)
	{
		DeliverySystem::getPostman()->insertMessage(applyForceMessages[i]);
	}
}

void OutgoingMessageBuffer::clearAllMessages()
{
	messages.clear();
	playerInputMessages.clear();
	textMessages.clear();
	sceneNodeTranslationMessages.clear();
	playSoundMessages.clear();
	stopSoundMessages.clear();
	graphicsModuleMessages.clear();
	applyForceMessages.clear();
}