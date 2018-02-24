#pragma once

#include "Message.h"
#include "Messages/PlayerInputMessage.h"
#include "Messages/TextMessage.h"
#include "Messages/PlaySoundMessage.h"
#include "Messages/StopSoundMessage.h"
#include "Messages/MoveCameraRelativeToGameObjectMessage.h"
#include "Messages/PlayMovingSoundMessage.h"
#include "MessagingService.h"
#include "MessageStorage.h"
#include "Messages/ApplyForceMessage.h"
#include "MessageBuffer.h"

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
	void insertMessage(PlaySoundMessage message) override;
	void insertMessage(StopSoundMessage message) override;
	void insertMessage(ToggleGraphicsModuleMessage message) override;
	void insertMessage(ApplyForceMessage message) override;
	void insertMessage(MoveCameraRelativeToGameObjectMessage message) override;
	void insertMessage(CollisionMessage message) override;
	void insertMessage(PreparePaintSurfaceMessage message) override;

	void deliverAllMessages() override;
	void clearAllMessages() override;

private:
	MessageStorage messageStorage;

	MessageBuffer<Message> messageBuffer;
	MessageBuffer<PlayerInputMessage> playerInputMessageBuffer;
	MessageBuffer<TextMessage> textMessageBuffer;
	MessageBuffer<RelativeTransformMessage> relativeTransformMessageBuffer;
	MessageBuffer<PlaySoundMessage> playSoundMessageBuffer;
	MessageBuffer<StopSoundMessage> stopSoundMessageBuffer;
	MessageBuffer<ToggleGraphicsModuleMessage> toggleGraphicsModuleMessageBuffer;
	MessageBuffer<ApplyForceMessage> applyForceMessageBuffer;
	MessageBuffer<MoveCameraRelativeToGameObjectMessage> moveCameraBuffer;
	MessageBuffer<CollisionMessage> collisionBuffer;
	MessageBuffer<PreparePaintSurfaceMessage> preparePaintSurfaceBuffer;
};

