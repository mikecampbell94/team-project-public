#include "SoundSystem.h"

SoundSystem::SoundSystem(Database *database, unsigned int channels)
	: Subsystem("SoundSystem")
{
	initOpenAl();
	this->database = database;

	//Create a vector of OALSources

	for (unsigned int i = 0; i < channels; ++i) {
		 ALuint source;
		 alGenSources(1, &source);
		 ALenum error = alGetError();
		 if (error == AL_NO_ERROR) {
			 sources.push_back(new OALSource(source));			
		}
		 else {
			 break;			
		}		
	}

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("SoundSystem"));
	
	incomingMessages.addActionToExecuteOnMessage(MessageType::AUDIO_MESSAGE, [this, &database](Message* message)
	{
		AudioMessage* am = static_cast<AudioMessage*>(message);
	
		Sound *sound = static_cast<Sound*>(database->getTable("SoundTable")->getAllResources()->getResource(am->soundID));
		
		SoundSource ss;
		ss.setPriority(am->priority);
		ss.setSound(sound);
		ss.setVolume(am->volume);
		ss.setLooping(am->isLooping);
		ss.setRadius(am->radius);
		ss.setPitch(am->pitch);
		ss.setIsGlobal(am->isGlobal);
		
		AudioSources.push_back(ss);
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
		return;
	}
	std::cout << "OpenAL create successfully" << alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	Context = alcCreateContext(Device, NULL);
	alcMakeContextCurrent(Context);
	alGetError();//not sure if the function of getError will be initialized by this method
}

void SoundSystem::cleanupOpenAl()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(Context);
	alcCloseDevice(Device);
}

void SoundSystem::UpdateListener()
{
	if (listener)
	{
		cameraWorldMatrix = Matrix4::translation(camera->getPosition());
		Vector3 worldPos = camera->getPosition();

		Vector3 dirup[2];

		//forward
		dirup[0].x = cameraWorldMatrix.values[2];
		dirup[0].y = cameraWorldMatrix.values[6];
		dirup[0].z = cameraWorldMatrix.values[10];

		//up
		dirup[1].x = cameraWorldMatrix.values[1];
		dirup[1].y = cameraWorldMatrix.values[5];
		dirup[1].z = cameraWorldMatrix.values[9];

		alListenerfv(AL_POSITION, (float*)&worldPos);
		alListenerfv(AL_ORIENTATION, (float*)&dirup);
	}
}

void SoundSystem::Update(float msec)
{
	UpdateListener();

	for (std::vector<SoundSource>::iterator i = AudioSources.begin(); i != AudioSources.end(); i++)
	{
		FrameSources.push_back(*i);
		i->update(msec);
	}
	CullNodes();
	if (FrameSources.size() > sources.size())
	{
		std::sort(FrameSources.begin(), FrameSources.end(), SoundSource::compareSourcesByPriority);
		detachSources((FrameSources.begin() + (sources.size() + 1)), FrameSources.end());
		attachSources((FrameSources.begin() + (sources.size() + 1)), FrameSources.end());
	}
	else
	{
		attachSources(FrameSources.begin(), FrameSources.end());
	}
	FrameSources.clear();
}
void SoundSystem::CullNodes()
{
	for (std::vector<SoundSource>::iterator i = FrameSources.begin(); i != FrameSources.end();)
	{
		SoundSource e = (*i);
		float length;
		length = (cameraWorldMatrix.getPositionVector() - e.getPosition()).Length();
		if (length > e.getRadius() || !e.getSound() || e.getTimeLeft() < 0)
		{
			e.detachSource();
			i = FrameSources.erase(i);
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
