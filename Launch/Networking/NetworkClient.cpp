#include "NetworkClient.h"

#include "../Input/Players/PlayerBase.h"
#include "../Gameplay/GameplaySystem.h"
#include "../Communication/Messages/UpdatePositionMessage.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"
#include <ctime>

enum PACKET_TYPE
{
	APPLY_FORCE_PACKET,
	POSITION_PACKET
};

struct PlayerPacket
{
	int type;
	int id;
	float x;
	float y;
	float z;
};

const float UPDATE_FREQUENCY = 15.0f;

NetworkClient::NetworkClient(InputRecorder* keyboardAndMouse, Database* database,
	PlayerBase* playerbase, GameplaySystem* gameplay) : Subsystem("NetworkClient")
{
	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::APPLY_FORCE, MessageType::UPDATE_POSITION }, 
		DeliverySystem::getPostman()->getDeliveryPoint("NetworkClient"));

	incomingMessages.addActionToExecuteOnMessage(MessageType::APPLY_FORCE, [&serverConnection = serverConnection, &clientID = clientID](Message* message)
	{
		ApplyForceMessage* forceMessage = static_cast<ApplyForceMessage*>(message);

		PlayerPacket forcePacket;
		forcePacket.type = APPLY_FORCE_PACKET;
		forcePacket.id = clientID;
		forcePacket.x = forceMessage->force.x;
		forcePacket.y = forceMessage->force.y;
		forcePacket.z = forceMessage->force.z;

		//ENetPacket* packet = enet_packet_create(&forcePacket, sizeof(PlayerPacket), 0);
		//enet_peer_send(serverConnection, 0, packet);
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::UPDATE_POSITION, [&serverConnection = serverConnection,
		&clientID = clientID, &isConnected = isConnected](Message* message)
	{
		if (isConnected)
		{
			UpdatePositionMessage* forceMessage = static_cast<UpdatePositionMessage*>(message);

			PlayerPacket forcePacket;
			forcePacket.type = POSITION_PACKET;
			forcePacket.id = clientID;
			forcePacket.x = forceMessage->position.x;
			forcePacket.y = forceMessage->position.y;
			forcePacket.z = forceMessage->position.z;

			//ENetPacket* packet = enet_packet_create(&forcePacket, sizeof(PlayerPacket), 0);
			//enet_peer_send(serverConnection, 0, packet);
		}
	});

	this->keyboardAndMouse = keyboardAndMouse;
	this->playerbase = playerbase;
	this->gameplay = gameplay;
	this->database = database;
	isNetworkUp = false;
	isConnected = false;
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::updateSubsystem(const float& deltaTime)
{
	timeSinceLastBroadcast += deltaTime;
	msCounter += deltaTime;

	if (timeSinceLastBroadcast >= UPDATE_FREQUENCY && isConnected)
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

	float currentTime = (float)(std::clock() / CLOCKS_PER_SEC);

	for (auto client = clientStates.begin(); client != clientStates.end(); ++client)
	{
		float factor = (msCounter - client->second.timeStamp) / UPDATE_FREQUENCY;

		if (factor <= 1.0f && factor >= 0.0f)
		{
			GameObject* clientGameObject = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(client->first));
			//clientGameObject->getPhysicsNode()->factor = factor;
			DeadReckoning::blendStates(clientGameObject->getPhysicsNode(), client->second, factor);
		}
	}

	if (isNetworkUp)
	{
		network.ServiceNetwork(0, [&serverConnection = serverConnection, &gameplay = gameplay,
			&keyboardAndMouse = keyboardAndMouse, &playerbase = playerbase, &clientID = clientID, 
			&isConnected = isConnected, &clientStates = clientStates, &database = database,
			&msCounter = msCounter](const ENetEvent& evnt)
		{
			switch (evnt.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
			{
				if (evnt.peer == serverConnection)
				{
					//Send a 'hello' packet
					char* text_data = "Connected to server";
					ENetPacket* packet = enet_packet_create(text_data, strlen(text_data) + 1, 0);
					enet_peer_send(serverConnection, 0, packet);
				}
			}
			break;

			case ENET_EVENT_TYPE_RECEIVE:
			{
				printf("\t Server %d says: %s\n", evnt.peer->incomingPeerID, evnt.packet->data);

				if (evnt.packet->dataLength == sizeof(int))
				{
					memcpy(&clientID, evnt.packet->data, sizeof(int));

					playerbase->addNewPlayer(keyboardAndMouse, clientID);
					gameplay->connectPlayerbase(playerbase);
					isConnected = true;
				}
				else if (evnt.packet->dataLength == sizeof(KinematicState) && isConnected)
				{
					KinematicState recievedState;
					memcpy(&recievedState, evnt.packet->data, sizeof(KinematicState));

					if (recievedState.id != clientID)
					{
						const std::string playerName = "player" + to_string(recievedState.id);

						GameObject* client = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(playerName));
						client->getPhysicsNode()->constantForce = true;
						recievedState.timeStamp = msCounter;
						
						client->getPhysicsNode()->startPosition = client->getPhysicsNode()->getPosition();
						client->getPhysicsNode()->startVelocity = client->getPhysicsNode()->getLinearVelocity();
						client->getPhysicsNode()->startAcceleration = client->getPhysicsNode()->getAcceleration();
						

						if (clientStates.find(playerName) != clientStates.end())
						{
							clientStates.at(playerName) = recievedState;
						}
						else
						{
							clientStates.insert({ playerName, recievedState });
						}
					}
				}

				enet_packet_destroy(evnt.packet);
			}
			break;
			}
		});
	}
}

void NetworkClient::connectToServer()
{
	if (network.Initialize(0))
	{
		serverConnection = network.ConnectPeer(10, 70, 33, 11, 1234);
		isNetworkUp = true;
	}
}
