#include "PlaySoundMessage.h"

PlaySoundMessage::PlaySoundMessage(const std::string& destinationName, NCLVector3 position,
	 SoundPriority priority, float volume, float radius, 
	float pitch, bool isLooping, std::string soundID, std::string soundNodeIdentifier)
	: Message(destinationName,PLAY_SOUND)
{
	this->position = position;
	this->priority = priority;
	this->volume = volume;
	this->radius = radius;
	this->pitch = pitch;
	this->isLooping = isLooping;
	this->soundID = soundID;
	this->soundNodeIdentifier = soundNodeIdentifier;
}

PlaySoundMessage::~PlaySoundMessage()
{

}
