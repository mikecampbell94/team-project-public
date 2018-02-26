#include "NetworkClient.h"

#include "../Input/Players/PlayerBase.h"
#include "../Gameplay/GameplaySystem.h"
#include "../Communication/Messages/UpdatePositionMessage.h"

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

NetworkClient::NetworkClient(InputRecorder* keyboardAndMouse,
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

		ENetPacket* packet = enet_packet_create(&forcePacket, sizeof(PlayerPacket), 0);
		enet_peer_send(serverConnection, 0, packet);
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

			ENetPacket* packet = enet_packet_create(&forcePacket, sizeof(PlayerPacket), 0);
			enet_peer_send(serverConnection, 0, packet);
		}
	});

	this->keyboardAndMouse = keyboardAndMouse;
	this->playerbase = playerbase;
	this->gameplay = gameplay;
	isNetworkUp = false;
	isConnected = false;
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::updateSubsystem(const float& deltaTime)
{
	if (isNetworkUp)
	{
		network.ServiceNetwork(0, [&serverConnection = serverConnection, &gameplay = gameplay,
			&keyboardAndMouse = keyboardAndMouse, &playerbase = playerbase, &clientID = clientID, 
			&isConnected = isConnected](const ENetEvent& evnt)
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
				else if (evnt.packet->dataLength == sizeof(PlayerPacket) && isConnected)
				{
					PlayerPacket recievedForcePacket;
					memcpy(&recievedForcePacket, evnt.packet->data, sizeof(PlayerPacket));

					if (recievedForcePacket.id != clientID)
					{
						if (recievedForcePacket.type == APPLY_FORCE_PACKET)
						{
							DeliverySystem::getPostman()->insertMessage(ApplyForceMessage("Physics", "player" + to_string(recievedForcePacket.id),
								Vector3(recievedForcePacket.x, recievedForcePacket.y, recievedForcePacket.z)));
						}
						else if(recievedForcePacket.type == POSITION_PACKET)
						{
							DeliverySystem::getPostman()->insertMessage(UpdatePositionMessage("Physics", "player" + to_string(recievedForcePacket.id),
								Vector3(recievedForcePacket.x, recievedForcePacket.y, recievedForcePacket.z)));
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
