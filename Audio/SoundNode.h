#pragma once

#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Matrix4.h"
#include "Sound.h"

class PlaySoundMessage;
class StopSoundMessage;

enum SoundPriority 
{
	SOUNDPRIORTY_LOW,
	SOUNDPRIORITY_MEDIUM,
	SOUNDPRIORITY_HIGH,
	SOUNDPRIORITY_ALWAYS
};

struct OALSource
{
	ALuint source;
	bool inUse;
	
	OALSource(ALuint src) 
	{
		source = src;
		inUse = false;
	}
};

class SoundNode
{
public:
	//SoundNode() = default;
	SoundNode(Sound* sound, NCLVector3 position, SoundPriority priority, float volume, 
		bool isLooping, float radius, float pitch, bool isGlobal, 
		std::string identifier);
	~SoundNode();

	static SoundNode builder(PlaySoundMessage* message, Sound* sound);

	float getRadius() const
	{
		return radius;
	}

	bool getIsGlobal() const
	{
		return isGlobal;
	}

	OALSource* getSource() const
	{
		return oalSource;
	}

	bool hasSound() const
	{
		return sound != nullptr;
	}

	double getTimeLeft() const
	{
		return timeLeft;
	}

	bool getIsLooping() const
	{
		return isLooping;
	}

	NCLVector3 getPosition() const
	{
		return position;
	}

	static bool compareSourcesByPriority(SoundNode& a, SoundNode& b);

	void attachSource(OALSource* s);
	void detachSource();

	void update(float msec);

	bool enabled = false;
	std::string identifier;

private:
	void setSound(Sound *s);

	void setVolume(float volume)
	{
		this->volume = min(1.0f, max(0.0f, volume));
	}

	void setRadius(float value)
	{
		radius = max(0.0f, value);
	}

	Sound* sound;
	NCLVector3 position;
	OALSource* oalSource;
	SoundPriority priority;
	float volume;
	float radius;
	float pitch;
	bool isLooping;
	bool isGlobal;
	double timeLeft;
};

