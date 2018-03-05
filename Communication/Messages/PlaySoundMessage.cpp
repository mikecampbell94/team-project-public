#include "PlaySoundMessage.h"

PlaySoundMessage::PlaySoundMessage(const std::string& destinationName, MessageType type, NCLVector3 position,
	 SoundPriority priority, float volume, float radius, 
	float pitch, bool isLooping, bool isGlobal, std::string soundID, std::string soundNodeIdentifier)
	: Message(destinationName,type)
{
	this->position = position;
	this->priority = priority;
	this->volume = volume;
	this->radius = radius;
	this->pitch = pitch;
	this->isLooping = isLooping;
	this->isGlobal = isGlobal;	
	this->soundID = soundID;
	this->soundNodeIdentifier = soundNodeIdentifier;
}

PlaySoundMessage::~PlaySoundMessage()
{

}
