#include "SoundSource.h"

SoundSource::SoundSource()
{
	reset();
}

SoundSource::SoundSource(Sound *s, Vector3 position, float volume)
{
	reset();
	sound = s;
	this->position = position;
	this->volume = volume;
}

SoundSource::~SoundSource()
{
}

void SoundSource::reset()
{
	position = Vector3(0.0f, 0.0f, 0.0f);
	priority = SOUNDPRIORTY_LOW;
	volume = 1.0f;
	radius = 500.0f;
	timeLeft = 0.0f;
	isLooping = false;
	oalSource = NULL;
	sound = NULL;
}

bool SoundSource::compareSourcesByPriority(SoundSource &a, SoundSource &b) 
{
	return (a.priority > b.priority) ? true : false;
}

void SoundSource::setSound(Sound * s)
{
	sound = s;
	detachSource();
	if (s) 
	{
		timeLeft = s->getLength();
	}
}

void SoundSource::attachSource(OALSource *s)
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

void SoundSource::detachSource()
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

void SoundSource::update(float msec)
{
	timeLeft -= msec;
	
	while (isLooping && timeLeft < 0.0f) 
	{
		timeLeft += sound->getLength();
	}
	
	if (oalSource) 
	{
		ALfloat listenerPos[] = { position.x, position.y, position.z };

		alSourcefv(oalSource->source, AL_POSITION, listenerPos);
		alSourcef(oalSource->source, AL_GAIN, volume);
		alSourcei(oalSource->source, AL_LOOPING, isLooping ? 1 : 0);
		alSourcef(oalSource->source, AL_MAX_DISTANCE, radius);
		alSourcef(oalSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);
	}
}
