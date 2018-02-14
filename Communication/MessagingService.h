#pragma once

#include "Message.h"
#include "Messages/PlayerInputMessage.h"
#include "Messages/TextMessage.h"

#include <string>
#include <queue>
#include "Messages/SceneNodeTranslationMessage.h"


//exposed interface
class MessagingService 
{
public:
	virtual void addDeliveryPoint(const std::string& bufferName) = 0;
	virtual std::queue<Message*>* getDeliveryPoint(const std::string& bufferName) = 0;

	virtual void insertMessage(Message message) = 0;
	virtual void insertMessage(PlayerInputMessage message) = 0;
	virtual void insertMessage(TextMessage message) = 0;
	virtual void insertMessage(SceneNodeTranslationMessage message) = 0;

	virtual void deliverAllMessages() = 0;
	virtual void clearAllMessages() = 0;
};