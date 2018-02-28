#include "NetworkClient.h"

#include "../Input/Players/PlayerBase.h"
#include "../Gameplay/GameplaySystem.h"
#include "../Communication/Messages/UpdatePositionMessage.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"
#include "NetworkMessageProcessor.h"
#include <ctime>

const float UPDATE_FREQUENCY = 50.0f;
const float UPDATE_TIMESTEP = 1.0f / 60.0f;

NetworkClient::NetworkClient(InputRecorder* keyboardAndMouse, Database* database,
	PlayerBase* playerbase, GameplaySystem* gameplay) : Subsystem("NetworkClient")
{
	incomingMessages = MessageProcessor(std::vector<MessageType> {}, 
		DeliverySystem::getPostman()->getDeliveryPoint("NetworkClient"));

	this->keyboardAndMouse = keyboardAndMouse;
	this->playerbase = playerbase;
	this->gameplay = gameplay;
	this->database = database;
	connectedToServer = false;
	joinedGame = false;
	updateRealTimeAccum = 0.0f;
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::updateSubsystem(const float& deltaTime)
{
	timeSinceLastBroadcast += deltaTime;
	msCounter += deltaTime;
	updateRealTimeAccum += deltaTime;

	if (timeSinceLastBroadcast >= UPDATE_FREQUENCY && joinedGame)
	{
		broadcastKinematicState();
	}

	if (updateRealTimeAccum >= UPDATE_TIMESTEP)
	{
		updateDeadReckoningForConnectedClients();
		updateRealTimeAccum = 0.0f;
	}

	if (connectedToServer)
	{
		processNetworkMessages(deltaTime);
	}
}

void NetworkClient::connectToServer()
{
	if (network.Initialize(0))
	{
		serverConnection = network.ConnectPeer(10, 70, 33, 11, 1234);
		connectedToServer = true;
	}
}

void NetworkClient::broadcastKinematicState()
{
	timeSinceLastBroadcast = 0.0f;

	const std::string playerName = "player" + to_string(clientID);
	GameObject* client = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(playerName));

	KinematicState state;
	state.id = clientID;
	state.position = client->getPhysicsNode()->getPosition();
	state.linearVelocity = client->getPhysicsNode()->getLinearVelocity();
	state.linearAcceleration = client->getPhysicsNode()->getAcceleration();

	ENetPacket* packet = enet_packet_create(&state, sizeof(KinematicState), 0);
	enet_peer_send(serverConnection, 0, packet);
}

void NetworkClient::updateDeadReckoningForConnectedClients()
{
	for (auto client = clientDeadReckonings.begin(); client != clientDeadReckonings.end(); ++client)
	{
		client->second.predictPosition(UPDATE_TIMESTEP);
		client->second.blendStates(client->first->getPhysicsNode());
	}
}

void NetworkClient::processNetworkMessages(const float& deltaTime)
{
	network.ServiceNetwork(deltaTime, [&serverConnection = serverConnection, &gameplay = gameplay,
		&keyboardAndMouse = keyboardAndMouse, &playerbase = playerbase, &clientID = clientID,
		&joinedGame = joinedGame, &database = database,
		&msCounter = msCounter, &clientDeadReckonings = clientDeadReckonings](const ENetEvent& evnt)
	{
		if (evnt.type == ENET_EVENT_TYPE_RECEIVE)
		{
			if (evnt.packet->dataLength == sizeof(int))
			{
				clientID = NetworkMessageProcessor::getClientNumber(evnt.packet);
				NetworkMessageProcessor::joinGame(clientID, playerbase, gameplay, keyboardAndMouse);
				joinedGame = true;
			}
			else if (evnt.packet->dataLength == sizeof(KinematicState) && joinedGame)
			{
				KinematicState recievedState;
				memcpy(&recievedState, evnt.packet->data, sizeof(KinematicState));

				if (recievedState.id != clientID)
				{
					const std::string playerName = "player" + to_string(recievedState.id);

					GameObject* client = NetworkMessageProcessor::getUpdatedDeadReckoningGameObject(playerName,
						recievedState, database);

					clientDeadReckonings[client] = DeadReckoning(recievedState);
				}
			}
		}

		enet_packet_destroy(evnt.packet);
	});
}