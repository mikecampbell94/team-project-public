#pragma once

#include "../Systems/Subsystem.h"
#include "../Input/Recorders/InputRecorder.h"
#include <Utilities/NetworkBase.h>
#include <enet\enet.h>

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
	int clientID;

	NetworkBase network;
	ENetPeer* serverConnection;

	InputRecorder* keyboardAndMouse;
	PlayerBase* playerbase; 
	GameplaySystem* gameplay;

	bool isNetworkUp;
	bool isConnected;
};

