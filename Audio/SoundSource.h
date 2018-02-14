#pragma once

#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Matrix4.h"
#include "Sound.h"

#include <algorithm>

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

class SoundSource
{
public:
	SoundSource();
	SoundSource(Sound *s, Vector3 position, float volume);
	~SoundSource();

	void reset();

	void setSound(Sound *s);
	Sound* getSound() { return sound; }

	void setPriority(SoundPriority p) { priority = p; }
	SoundPriority getPriority() { return priority; }

	void setVolume(float volume) { volume = min(1.0f, max(0.0f, volume)); }
	float getVolume() { return volume; }

	void setLooping(bool state) { isLooping = state; }
	bool getLooping() { return isLooping; }

	void setRadius(float value) { radius = max(0.0f, value); }
	float getRadius() { return radius; }

	float getPitch() { return pitch; }
	void setPitch(float value) { pitch = value; }

	bool getIsGlobal() { return isGlobal; }
	void setIsGlobal(bool value) { isGlobal = value; }

	double getTimeLeft() { return timeLeft; }

	OALSource* getSource() { return oalSource; }

	static bool compareSourcesByPriority(SoundSource &a, SoundSource &b);

	void attachSource(OALSource* s);
	void detachSource();

	void update(float msec);

	Vector3 getPosition() { return position; }
	void setPosition(Vector3 position) { this->position = position; }

private:
	Sound* sound;
	Vector3 position;
	OALSource* oalSource;
	SoundPriority priority;
	float volume;
	float radius;
	float pitch;
	bool isLooping;
	bool isGlobal;
	double timeLeft;
};

