#include "AudioSystem.h"

AudioSystem::AudioSystem(Database *database, Camera *camera) : Subsystem("AudioSystem")
{
	soundManager = std::make_unique<SoundManager>(database, camera);

	incomingMessages = MessageProcessor(std::vector<MessageType>{ MessageType::PLAY_SOUND, MessageType::STOP_SOUND }, DeliverySystem::getPostman()->getDeliveryPoint("AudioSystem"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::PLAY_SOUND, [&soundManagerPointer = soundManager, database](Message* message)
	{
		PlaySoundMessage* soundMessage = static_cast<PlaySoundMessage*>(message);
		soundManagerPointer->AddNewSoundNode(soundMessage);
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::STOP_SOUND, [&soundManagerPointer = soundManager](Message* message)
	{
		StopSoundMessage* soundMessage = static_cast<StopSoundMessage*>(message);
		soundManagerPointer->stopSoundNode(soundMessage);
	});
}

void AudioSystem::updateSubsystem(const float& deltaTime)
{
	soundManager->update(deltaTime);
}
