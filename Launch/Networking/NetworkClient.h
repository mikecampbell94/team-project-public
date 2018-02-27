#pragma once

#include "../Systems/Subsystem.h"
#include "../Input/Recorders/InputRecorder.h"
#include <Utilities/NetworkBase.h>
#include <enet\enet.h>
#include <set>
#include "DeadReckoning.h"

class GameplaySystem;
class PlayerBase;
class Database;

class NetworkClient : public Subsystem
{
public:
	NetworkClient(InputRecorder* keyboardAndMouse, Database* database,
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
	Database* database;

	bool isNetworkUp;
	bool isConnected;
	std::map<std::string, KinematicState> clientStates;
	float timeSinceLastBroadcast = 0.0f;
	float msCounter = 0.0f;
};

