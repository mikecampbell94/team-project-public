#include "SoundNode.h"

#include "../Communication/Messages/PlaySoundMessage.h"

SoundNode::SoundNode(Sound* sound, Vector3 position, SoundPriority priority, float volume,
	bool isLooping, float radius, float pitch, bool isGlobal,
	std::string identifier)
{
	this->position = position;
	this->priority = priority;
	setVolume(volume);
	this->isLooping = isLooping;
	setRadius(radius);
	this->pitch = pitch;
	this->isGlobal = isGlobal;
	this->identifier = identifier;
	timeLeft = 0.0f;
	enabled = true;
	oalSource = nullptr;
	setSound(sound);
}

SoundNode::~SoundNode()
{
}

SoundNode SoundNode::builder(PlaySoundMessage* message, Sound* sound)
{
	SoundNode soundNode(sound, message->position, message->priority, message->volume,
		message->isLooping, message->radius, message->pitch, message->isGlobal,
		message->soundNodeIdentifier);
	soundNode.enabled = true;

	return soundNode;
}

bool SoundNode::compareSourcesByPriority(SoundNode& a, SoundNode& b)
{
	if (a.priority > b.priority)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SoundNode::setSound(Sound * s)
{
	sound = s;
	detachSource();

	if (s) 
	{
		timeLeft = s->getLength();
	}
}

void SoundNode::attachSource(OALSource *s)
{
	oalSource = s;
	
	if (!oalSource)
	{
		return;
	}

	oalSource->inUse = true;
	
	alSourcef(oalSource->source, AL_MAX_DISTANCE, radius);
	alSourcef(oalSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);
	alSourcei(oalSource->source, AL_BUFFER, sound->getBuffer());
	alSourcef(oalSource->source, AL_SEC_OFFSET, (sound->getLength() / 1000.0) - (timeLeft / 1000.0));
	alSourcePlay(oalSource->source);
}

void SoundNode::detachSource()
{
	if (!oalSource) 
	{
		return;
	}
	
	alSourcef(oalSource->source, AL_GAIN, 0.0f);
	alSourceStop(oalSource->source);
	alSourcei(oalSource->source, AL_BUFFER, 0);
	oalSource->inUse = false;
	oalSource = NULL;
}

void SoundNode::update(float msec)
{
	timeLeft -= msec;
	
	while (isLooping && timeLeft < 0.0f) 
	{
		timeLeft += sound->getLength();
	}
	
	if (oalSource) 
	{
		ALfloat pos[] = { position.x, position.y, position.z };

		alSourcefv(oalSource->source, AL_POSITION, pos);
		alSourcef(oalSource->source, AL_GAIN, volume);
		alSourcei(oalSource->source, AL_LOOPING, isLooping ? 1 : 0);
		alSourcef(oalSource->source, AL_MAX_DISTANCE, radius);
		alSourcef(oalSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);
	}
}