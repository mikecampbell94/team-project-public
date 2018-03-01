#pragma once

#include "Message.h"
#include "Messages/PlayerInputMessage.h"
#include "Messages/TextMessage.h"
#include "Messages/PlaySoundMessage.h"
#include "Messages/StopSoundMessage.h"
#include <string>
#include <queue>
#include "Messages/RelativeTransformMessage.h"
#include "Messages/ToggleGraphicsModuleMessage.h"
#include "Messages/ApplyForceMessage.h"
#include "Messages/MoveCameraRelativeToGameObjectMessage.h"
#include "Messages/ApplyImpulseMessage.h"

#include "Messages/CollisionMessage.h"
#include "Messages/PreparePaintSurfaceMessage.h"
#include "Messages/PaintTrailForGameObjectMessage.h"
#include "Messages/UpdatePositionMessage.h"


//exposed interface
class MessagingService 
{
public:
	virtual void addDeliveryPoint(const std::string& bufferName) = 0;
	virtual std::queue<Message*>* getDeliveryPoint(const std::string& bufferName) = 0;

	virtual void insertMessage(Message message) = 0;
	virtual void insertMessage(PlayerInputMessage message) = 0;
	virtual void insertMessage(TextMessage message) = 0;
	virtual void insertMessage(RelativeTransformMessage message) = 0;	virtual void insertMessage(PlaySoundMessage message) = 0;
	virtual void insertMessage(StopSoundMessage message) = 0;
	virtual void insertMessage(ToggleGraphicsModuleMessage message) = 0;
	virtual void insertMessage(ApplyForceMessage message) = 0;
	virtual void insertMessage(ApplyImpulseMessage message) = 0;
	virtual void insertMessage(MoveCameraRelativeToGameObjectMessage message) = 0;
	virtual void insertMessage(CollisionMessage message) = 0;
	virtual void insertMessage(PreparePaintSurfaceMessage message) = 0;
	virtual void insertMessage(PaintTrailForGameObjectMessage message) = 0;
	virtual void insertMessage(UpdatePositionMessage message) = 0;
	virtual void deliverAllMessages() = 0;
	virtual void clearAllMessages() = 0;
};