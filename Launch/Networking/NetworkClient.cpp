#include "NetworkClient.h"

#include "../Input/Players/PlayerBase.h"
#include "../Gameplay/GameplaySystem.h"
#include "../Communication/Messages/UpdatePositionMessage.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"
#include <ctime>

const float UPDATE_FREQUENCY = 50.0f;
const float MAX_INTERPOLATION_DISTANCE = 50.0f;
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

	updateDeadReckoningForConnectedClients();

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
	state.timeStamp = msCounter;

	ENetPacket* packet = enet_packet_create(&state, sizeof(KinematicState), 0);
	enet_peer_send(serverConnection, 0, packet);
}

void NetworkClient::updateDeadReckoningForConnectedClients()
{
	if (updateRealTimeAccum >= UPDATE_TIMESTEP)
	{
		for (auto client = clientDeadReckonings.begin(); client != clientDeadReckonings.end(); ++client)
		{
			GameObject* clientGameObject = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(client->first));

			client->second.predictPosition(UPDATE_TIMESTEP);

			float factor = (client->second.prediction.position - clientGameObject->getPhysicsNode()->getPosition()).length();
			factor /= MAX_INTERPOLATION_DISTANCE;

			if (factor > 1.0f)
			{
				factor = 1.0f;
			}

			client->second.blendStates(clientGameObject->getPhysicsNode(), factor);
		}

	}
	updateRealTimeAccum = 0.0f;
}

void NetworkClient::processNetworkMessages(const float& deltaTime)
{
	network.ServiceNetwork(deltaTime, [&serverConnection = serverConnection, &gameplay = gameplay,
		&keyboardAndMouse = keyboardAndMouse, &playerbase = playerbase, &clientID = clientID,
		&joinedGame = joinedGame, &clientStates = clientStates, &database = database,
		&msCounter = msCounter, &clientDeadReckonings = clientDeadReckonings](const ENetEvent& evnt)
	{
		switch (evnt.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
		{
			if (evnt.packet->dataLength == sizeof(int))
			{
				memcpy(&clientID, evnt.packet->data, sizeof(int));

				playerbase->addNewPlayer(keyboardAndMouse, clientID);
				gameplay->connectPlayerbase(playerbase);
				joinedGame = true;
			}
			else if (evnt.packet->dataLength == sizeof(KinematicState) && joinedGame)
			{
				KinematicState recievedState;
				memcpy(&recievedState, evnt.packet->data, sizeof(KinematicState));

				if (recievedState.id != clientID)
				{
					const std::string playerName = "player" + to_string(recievedState.id);

					GameObject* client = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(playerName));
					client->getPhysicsNode()->constantForce = true;
					recievedState.timeStamp = msCounter;

					client->getPhysicsNode()->setLinearVelocity(recievedState.linearVelocity);
					client->getPhysicsNode()->setAcceleration(recievedState.linearAcceleration);

					DeadReckoning update;
					update.prediction = recievedState;

					if (clientStates.find(playerName) != clientStates.end())
					{
						clientStates.at(playerName) = recievedState;
					}
					else
					{
						clientStates.insert({ playerName, recievedState });
					}

					if (clientDeadReckonings.find(playerName) != clientDeadReckonings.end())
					{
						clientDeadReckonings.at(playerName) = update;
					}
					else
					{
						clientDeadReckonings.insert({ playerName, update });
					}
				}
			}

			enet_packet_destroy(evnt.packet);
		}
		break;
		}
	});
}
