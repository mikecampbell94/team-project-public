#pragma once

#include "Message.h"
#include "Messages/PlayerInputMessage.h"
#include "Messages/TextMessage.h"
#include "Messages/PlaySoundMessage.h"
#include "Messages/StopSoundMessage.h"
#include "Messages/PlayMovingSoundMessage.h"
#include "MessagingService.h"
#include "MessageStorage.h"
#include "Messages/ApplyForceMessage.h"

class OutgoingMessageBuffer : public MessagingService
{
public:
	OutgoingMessageBuffer();
	~OutgoingMessageBuffer();

	void insertMessage(Message message) override;
	void insertMessage(PlayerInputMessage message) override;
	void insertMessage(TextMessage message) override;
	void insertMessage(RelativeTransformMessage message) override;
	void insertMessage(PlaySoundMessage message) override;
	void insertMessage(StopSoundMessage message) override;
	void insertMessage(ToggleGraphicsModuleMessage message) override;
	void insertMessage(ApplyForceMessage message) override;
	void deliverAllMessages() override;
	void clearAllMessages() override;

private:
	void addDeliveryPoint(const std::string& bufferName) override {}
	std::queue<Message*>* getDeliveryPoint(const std::string& bufferName) override { return nullptr; }

	std::vector<Message> messages;
	std::vector<PlayerInputMessage> playerInputMessages;
	std::vector<TextMessage> textMessages;
	std::vector<RelativeTransformMessage> sceneNodeTranslationMessages;
	std::vector<PlaySoundMessage> playSoundMessages;
	std::vector<StopSoundMessage> stopSoundMessages;
	std::vector<ToggleGraphicsModuleMessage> graphicsModuleMessages;
	std::vector<ApplyForceMessage> applyForceMessages;
};

