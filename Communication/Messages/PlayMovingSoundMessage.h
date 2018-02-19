#pragma once

#include "../Communication/Message.h"
#include "../Audio/Sound.h"
#include "../Audio/SoundNode.h"

class PlayMovingSoundMessage : public Message
{
public:
	PlayMovingSoundMessage(const std::string& desinationName, MessageType type, Vector3 *position,
		SoundPriority priority, float volume, float radius, float pitch, bool isLooping,
		bool isGlobal, std::string soundID, std::string soundNodeIdentifier);
	~PlayMovingSoundMessage();

	Vector3 *position;
	SoundPriority priority;
	float volume;
	float radius;
	float pitch;
	bool isLooping;
	bool isGlobal;
	std::string soundID;
	std::string soundNodeIdentifier;
};

