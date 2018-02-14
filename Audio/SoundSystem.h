#pragma once
#include <vector>

#include "OpenAL 1.1 SDK\include\al.h"
#include "OpenAL 1.1 SDK\include\alc.h"

#include "../Launch/Systems/Subsystem.h"

#include "../Resource Management/Database/Database.h"

#include "../Communication/Message.h"
#include "../Communication/MessageProcessor.h"
#include "../Communication/DeliverySystem.h"

#include "../Utilities/Maths/Matrix4.h"
#include "../Graphics/Utility/Camera.h"

#include "SoundSource.h"
#include "Sound.h"

#include "../Communication/AudioMessage.h"

class SoundSystem : public Subsystem
{
public:
	SoundSystem(Database *database, Camera *camera, unsigned int channels = 128);
	~SoundSystem();

	void initOpenAl();
	void cleanupOpenAl();
	void update(float msec);
	
protected:
	ALCcontext *Context;
	ALCdevice *Device;
	std::vector<MessageType> types = { MessageType::AUDIO_MESSAGE };	
	std::vector<SoundSource> audioSources;
	std::vector<SoundSource> frameSources;
	Camera* camera;
	Database *database;
	Matrix4 cameraWorldMatrix;
	std::vector<OALSource*> sources;
	
	void updateListener();
	void cullNodes();
	void detachSources(std::vector<SoundSource>::iterator from, std::vector<SoundSource>::iterator to);
	void attachSources(std::vector<SoundSource>::iterator from, std::vector<SoundSource>::iterator to);
	OALSource* getSource();

};

