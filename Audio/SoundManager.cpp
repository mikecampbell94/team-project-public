#include "SoundManager.h"

#include "../Resource Management/Database/Database.h"
#include "../Communication/Messages/PlaySoundMessage.h"
#include "../Communication/Messages/StopSoundMessage.h"
#include "../Graphics/Utility/Camera.h"

const int CHANNELS = 128;
const int FORWARD_DIRECTION = 0;
const int UPWARDS_DIRECTION = 1;

SoundManager::SoundManager(Database* database, Camera* camera)
{
	initialiseOpenAl();
	createOALSources();

	this->database = database;
	this->camera = camera;
}

SoundManager::~SoundManager()
{
	alcMakeContextCurrent(NULL);

	for (vector<OALSource*>::iterator i = OALSources.begin(); i != OALSources.end(); ++i)
	{
		alDeleteSources(1, &(*i)->source);
		delete *i;
	}

	alcDestroyContext(context);
	alcCloseDevice(device);
}

void SoundManager::initialiseOpenAl()
{
	device = alcOpenDevice(NULL);
	if (!device)
	{
		std::cout << "OpenAL initialisation failed! No valid device" << std::endl;
		throw runtime_error("No OALDevice");
	}

	std::cout << "OpenAL initialised successfully! Device: " << alcGetString(NULL, ALC_DEVICE_SPECIFIER) << endl;

	context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
}

void SoundManager::createOALSources()
{
	for (unsigned int i = 0; i < CHANNELS; ++i)
	{
		ALuint source;
		alGenSources(1, &source);
		const ALenum error = alGetError();

		if (error == AL_NO_ERROR)
		{
			OALSources.push_back(new OALSource(source));
		}
		else
		{
			break;
		}
	}
}

void SoundManager::AddNewSoundNode(PlaySoundMessage* message)
{
	if(soundNodes.size() > 0)
	{
		for (unsigned int i = 0; i < soundNodes.size(); ++i)
		{
			if (soundNodes[i].identifier != message->soundNodeIdentifier)
			{
				Sound* sound = static_cast<Sound*>(database->getTable("SoundObjects")->getResource(message->soundID));
				soundNodes.push_back(SoundNode::builder(message, sound));
			}
		}
	}
	else
	{
		Sound* sound = static_cast<Sound*>(database->getTable("SoundObjects")->getResource(message->soundID));
		soundNodes.push_back(SoundNode::builder(message, sound));
	}
	
}

void SoundManager::stopSoundNode(StopSoundMessage* message)
{
	for (unsigned int i = 0; i < soundNodes.size(); ++i)
	{
		if (soundNodes[i].identifier == message->soundNodeIdentifier)
		{
			soundNodes[i].enabled = false;
			soundNodes[i].detachSource();
		}
	}
}


void SoundManager::update(const float& deltaTime)
{
	updateListenerToCameraPosition();

	for (SoundNode& node : soundNodes)
	{
		node.update(deltaTime);
	}

	cullNodes();

	if (soundNodes.size() <= OALSources.size())
	{
		attachSources(soundNodes.begin(), soundNodes.end());
	}
	else
	{
		std::sort(soundNodes.begin(), soundNodes.end(), SoundNode::compareSourcesByPriority);
		detachSources((soundNodes.begin() + (OALSources.size() + 1)), soundNodes.end());
		attachSources((soundNodes.begin() + (OALSources.size() + 1)), soundNodes.end());
	}

	removeSoundNodesFromSystem();
}

void SoundManager::updateListenerToCameraPosition()
{
	const NCLVector3 listenerWorldPos = camera->viewMatrix.getPositionVector();

	NCLVector3 orientation[2];

	orientation[FORWARD_DIRECTION].x = camera->viewMatrix.values[2];
	orientation[FORWARD_DIRECTION].y = camera->viewMatrix.values[6];
	orientation[FORWARD_DIRECTION].z = camera->viewMatrix.values[10];

	orientation[UPWARDS_DIRECTION].x = camera->viewMatrix.values[1];
	orientation[UPWARDS_DIRECTION].y = camera->viewMatrix.values[5];
	orientation[UPWARDS_DIRECTION].z = camera->viewMatrix.values[9];

	ALfloat listenerPos[] = { listenerWorldPos.x, listenerWorldPos.y, listenerWorldPos.z };
	ALfloat listenerOri[] = { 
		orientation[FORWARD_DIRECTION].x, orientation[FORWARD_DIRECTION].y, orientation[FORWARD_DIRECTION].z, 
		orientation[UPWARDS_DIRECTION].x, orientation[UPWARDS_DIRECTION].y, orientation[UPWARDS_DIRECTION].z };

	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

void SoundManager::cullNodes()
{
	for (SoundNode& node : soundNodes)
	{
		const float distanceBetweenListenerAndSoundNode = (camera->viewMatrix.getPositionVector() - node.getPosition()).length();

		if (distanceBetweenListenerAndSoundNode > node.getRadius() || !node.hasSound() || node.getTimeLeft() < 0)
		{
			if (!node.getIsLooping())
			{
				node.enabled = false;
				node.detachSource();
			}
			else
			{
				node.detachSource();
			}
		}
	}
}

void SoundManager::detachSources(std::vector<SoundNode>::iterator& from, std::vector<SoundNode>::iterator& to)
{
	for (std::vector<SoundNode>::iterator i = from; i != to; ++i)
	{
		(*i).detachSource();
	}
}

void SoundManager::attachSources(std::vector<SoundNode>::iterator& from, std::vector<SoundNode>::iterator& to)
{
	for (vector<SoundNode>::iterator i = from; i != to; ++i)
	{
		if (!(*i).getSource() && (*i).enabled)
		{
			(*i).attachSource(getOALSource());
		}
	}
}

OALSource* SoundManager::getOALSource()
{
	for (vector<OALSource*>::iterator i = OALSources.begin(); i != OALSources.end(); ++i)
	{
		if (!(*i)->inUse)
		{
			return (*i);
		}
	}

	//TECH_DEBT
	return nullptr;
}

void SoundManager::removeSoundNodesFromSystem()
{
	for (vector<SoundNode>::iterator i = soundNodes.begin(); i != soundNodes.end();)
	{
		if(!i->enabled && !i->getSource())
		{
			i = soundNodes.erase(i);
		}
		else
		{
			++i;
		}
	}
}

