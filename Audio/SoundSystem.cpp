#include "SoundSystem.h"

SoundSystem::SoundSystem(Database *database, Camera *camera, unsigned int channels)
	: Subsystem("SoundSystem")
{
	initOpenAl();
	this->database = database;
	this->camera = camera;

	cameraWorldMatrix = this->camera->viewMatrix;

	for (unsigned int i = 0; i < channels; ++i) 
	{
		 ALuint source;
		 alGenSources(1, &source);
		 ALenum error = alGetError();
		 if (error == AL_NO_ERROR) 
		 {
			 sources.push_back(new OALSource(source));			
		}
		 else 
		 {
			 break;			
		}		
	}

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("SoundSystem"));
	
	incomingMessages.addActionToExecuteOnMessage(MessageType::AUDIO_MESSAGE, [this, &database](Message* message)
	{
		AudioMessage* am = static_cast<AudioMessage*>(message);
	
		Sound *sound = static_cast<Sound*>(database->getTable("SoundTable")->getAllResources()->getResource(am->soundID));
		
		SoundSource ss;
		ss.setSound(sound);
		ss.setPosition(am->position);
		ss.setPriority(am->priority);
		ss.setVolume(am->volume);
		ss.setLooping(am->isLooping);
		ss.setRadius(am->radius);
		ss.setPitch(am->pitch);
		ss.setIsGlobal(am->isGlobal);
		
		audioSources.push_back(ss);
	});
}

SoundSystem::~SoundSystem()
{
	cleanupOpenAl();
}

void SoundSystem::initOpenAl()
{
	Device = alcOpenDevice(NULL);
	if (!Device)
	{
		std::cout << "OpenAL initialisation failed! No valid device" << std::endl;
		return;
	}
	std::cout << "OpenAL initialised successfully! Device: " << alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	Context = alcCreateContext(Device, NULL);
	alcMakeContextCurrent(Context);
}

void SoundSystem::cleanupOpenAl()
{
	alcMakeContextCurrent(NULL);

	for (vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i)
	{
		alDeleteSources(1, &(*i)->source);
		delete (*i);
	}

	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

void SoundSystem::updateListener()
{
	Vector3 worldPos = cameraWorldMatrix.getPositionVector();

	Vector3 dirup[2];

	//forward
	dirup[0].x = cameraWorldMatrix.values[2];
	dirup[0].y = cameraWorldMatrix.values[6];
	dirup[0].z = cameraWorldMatrix.values[10];

	//up
	dirup[1].x = cameraWorldMatrix.values[1];
	dirup[1].y = cameraWorldMatrix.values[5];
	dirup[1].z = cameraWorldMatrix.values[9];

	ALfloat listenerPos[] = { worldPos.x, worldPos.y, worldPos.z };
	ALfloat listenerOri[] = { dirup[0].x, dirup[0].y, dirup[0].z, dirup[1].x, dirup[1].y, dirup[1].z };

	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

void SoundSystem::update(float msec)
{
	updateListener();

	for (std::vector<SoundSource>::iterator i = audioSources.begin(); i != audioSources.end(); i++)
	{
		frameSources.push_back(*i);
		i->update(msec);
	}

	cullNodes();
	
	if (frameSources.size() > sources.size())
	{
		std::sort(frameSources.begin(), frameSources.end(), SoundSource::compareSourcesByPriority);
		detachSources((frameSources.begin() + (sources.size() + 1)), frameSources.end());
		attachSources((frameSources.begin() + (sources.size() + 1)), frameSources.end());
	}
	else
	{
		attachSources(frameSources.begin(), frameSources.end());
	}

	frameSources.clear();
}
void SoundSystem::cullNodes()
{
	for (std::vector<SoundSource>::iterator i = frameSources.begin(); i != frameSources.end();)
	{
		SoundSource e = (*i);
		float length = (cameraWorldMatrix.getPositionVector() - e.getPosition()).length();
		if (length > e.getRadius() || !e.getSound() || e.getTimeLeft() < 0)
		{
			e.detachSource();
			i = frameSources.erase(i);
		}
		else
		{
			++i;
		}		
	}
}

void SoundSystem::detachSources(std::vector<SoundSource>::iterator from, std::vector<SoundSource>::iterator to)
{
	for (std::vector<SoundSource>::iterator i = from; i != to; ++i)
	{
		(*i).detachSource();
	}
}

void SoundSystem::attachSources(std::vector<SoundSource>::iterator from, std::vector<SoundSource>::iterator to)
{
	for (vector<SoundSource>::iterator i = from; i != to; ++i)
	{
		if (!(*i).getSource())
		{
			(*i).attachSource(getSource());
		}
	}
}

OALSource* SoundSystem::getSource()
{
	for (vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i)
	{
		OALSource* s = (*i);
		if (!s->inUse)
		{
			return s;
		}
	}
	return nullptr;
}
