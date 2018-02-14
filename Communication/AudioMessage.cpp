#include "AudioMessage.h"

AudioMessage::AudioMessage(const std::string& destinationName, MessageType type, Vector3 position,
	 SoundPriority priority, float volume, float radius, 
	float pitch, bool isLooping, bool isGlobal, std::string soundID)
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
}

AudioMessage::~AudioMessage()
{
}
