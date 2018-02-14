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

#include "../Graphics/Scene Management/SceneNode.h"

class SoundSystem : public Subsystem
{
public:
	SoundSystem(Database *database, unsigned int channels = 128);
	~SoundSystem();

	void initOpenAl();
	void cleanupOpenAl();

	void UpdateListener();

	void Update(float msec);
	
protected:
	ALCcontext * Context;
	ALCdevice * Device;
	std::vector<MessageType> types = { MessageType::AUDIO_MESSAGE };	
	std::vector<SoundSource> AudioSources;
	std::vector<SoundSource> FrameSources;
	Camera* camera;
	Database *database;
	SceneNode* listener;
	//Matrix4 listenerTransform;
	Matrix4 cameraWorldMatrix;
	std::vector<OALSource*> sources;
		void CullNodes();	void detachSources(std::vector<SoundSource>::iterator from,
		std::vector<SoundSource>::iterator to);
	void attachSources(std::vector<SoundSource>::iterator from,		std::vector<SoundSource>::iterator to);	OALSource* getSource();
};

