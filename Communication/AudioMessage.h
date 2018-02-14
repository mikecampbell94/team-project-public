#pragma once

#include "../Communication/Message.h"
#include "../Audio/Sound.h"
#include "../Audio/SoundSource.h"

class AudioMessage : public Message
{
public:
	AudioMessage(const std::string& desinationName, MessageType type, Vector3 position,
		 SoundPriority priority, float volume, float radius, float pitch, bool isLooping,
		bool isGlobal, std::string soundID);
	~AudioMessage();

	Vector3 position;
	SoundPriority priority;
	float volume;
	float radius;
	float pitch;
	bool isLooping;
	bool isGlobal;
	std::string soundID;
};
