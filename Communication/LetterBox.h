#pragma once

#include "Message.h"
#include "Messages/PlayerInputMessage.h"
#include "Messages/TextMessage.h"
#include "MessagingService.h"
#include "MessageStorage.h"

class LetterBox : public MessagingService
{
public:
	LetterBox();
	virtual ~LetterBox();

	void addDeliveryPoint(const std::string& bufferName) override;
	std::queue<Message*>* getDeliveryPoint(const std::string& bufferName) override;

	void insertMessage(Message message) override;
	void insertMessage(PlayerInputMessage message) override;
	void insertMessage(TextMessage message) override;
	void insertMessage(RelativeTransformMessage message) override;

	void deliverAllMessages() override;
	void clearAllMessages() override;

private:
	MessageStorage messageStorage;

	std::vector<Message> messages;
	std::vector<PlayerInputMessage> playerInputMessages;
	std::vector<TextMessage> textMessages;
	std::vector<RelativeTransformMessage> sceneNodeTranslationMessages;
};

