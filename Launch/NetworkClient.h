#pragma once

#include "Systems\Subsystem.h"
#include <enet\enet.h>
#include <Utilities/NetworkBase.h>
#include "../Input/Recorders/InputRecorder.h"

class GameplaySystem;
class PlayerBase;

class NetworkClient : public Subsystem
{
public:
	NetworkClient(InputRecorder* keyboardAndMouse, 
		PlayerBase* playerbase, GameplaySystem* gameplay);
	~NetworkClient();

	void updateSubsystem(const float& deltaTime) override;

	void connectToServer();

private:
	NetworkBase network;
	ENetPeer* serverConnection;

	InputRecorder* keyboardAndMouse;
	PlayerBase* playerbase; 
	GameplaySystem* gameplay;

	bool isConnected;
};

