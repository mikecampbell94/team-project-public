#include "NetworkClient.h"

#include "../Input/Players/PlayerBase.h"
#include "../Gameplay/GameplaySystem.h"
#include "../Communication/Messages/UpdatePositionMessage.h"
#include "../Resource Management/Database/Database.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"
#include "NetworkMessageProcessor.h"
#include <ctime>
#include "../Utilities/GameTimer.h"
#include "Scripting/PaintGameActionBuilder.h"

enum
{
	NEW_ID,
	CURRENT_NUMBER_OF_PLAYERS
};

struct IntegerData
{
	int type;
	int data;
};



const float UPDATE_FREQUENCY = 50.0f;
const float UPDATE_TIMESTEP = 1.0f / 60.0f;

NetworkClient::NetworkClient(InputRecorder* keyboardAndMouse, Database* database,
	PlayerBase* playerbase, GameplaySystem* gameplay) : Subsystem("NetworkClient")
{
	incomingMessages = MessageProcessor(std::vector<MessageType> {MessageType::TEXT}, 
		DeliverySystem::getPostman()->getDeliveryPoint("NetworkClient"));


	this->keyboardAndMouse = keyboardAndMouse;
	this->playerbase = playerbase;
	this->gameplay = gameplay;
	this->database = database;
	connectedToServer = false;
	joinedGame = false;
	updateRealTimeAccum = 0.0f;

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [&database = this->database, &objectsToToTransmitStatesFor = objectsToToTransmitStatesFor,
	&transmitMinionColour = transmitMinionColour, &minionColour = minionColour](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);

		istringstream iss(textMessage->text);
		vector<string> tokens{ istream_iterator<string>{iss},
			std::istream_iterator<string>{} };

		if (tokens[0] == "insertMinion")
		{
			GameObject* gameObject = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(tokens[1]));

			objectsToToTransmitStatesFor.push_back(gameObject);
		}
		else if (tokens[0] == "paintMinion")
		{
			for (int i = 0; i < objectsToToTransmitStatesFor.size(); ++i)
			{
				if (objectsToToTransmitStatesFor[i]->getName() == tokens[1])
				{
					minionColour.minionIndex = i;
					minionColour.minionColour = objectsToToTransmitStatesFor[i]->stats.colourToPaint;
					transmitMinionColour = true;
					break;
				}
			}
		}

		
	});

	waitingInLobbyText = PeriodicTextModifier("Waiting for players", ".", 3);

	timer->addChildTimer("Broadcast Kinematic State");
	timer->addChildTimer("Dead Reckoning");
	timer->addChildTimer("Process Network Messages");
}

NetworkClient::~NetworkClient()
{
}

void NetworkClient::updateNextFrame(const float& deltaTime)
{
	timer->beginTimedSection();

	if (!inLobby)
	{
		DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "deltatime enable"));
		timeSinceLastBroadcast += deltaTime;
		msCounter += deltaTime;
		updateRealTimeAccum += deltaTime;

		timer->beginChildTimedSection("Broadcast Kinematic State");
		if (timeSinceLastBroadcast >= UPDATE_FREQUENCY && joinedGame)
		{
			broadcastKinematicState();

			if (clientID == 0)
			{
				broadcastMinionState();
			}
		}
		timer->endChildTimedSection("Broadcast Kinematic State");

		timer->beginChildTimedSection("Dead Reckoning");
		if (updateRealTimeAccum >= UPDATE_TIMESTEP)
		{
			updateDeadReckoningForConnectedClients();

			if (clientID != 0)
			{
				updateDeadReckoningForMinions();
			}

			updateRealTimeAccum = 0.0f;
		}
		timer->endChildTimedSection("Dead Reckoning");

		for (int i = 0; i < numberOfOtherPlayersToWaitFor; ++i)
		{
			const std::string playerName = "player" + to_string(i);
			GameObject* client = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(playerName));

			DeliverySystem::getPostman()->insertMessage(TextMeshMessage("RenderingSystem", playerName,
				client->getPhysicsNode()->getPosition() + NCLVector3(24, 14, 0), NCLVector3(7, 7, 1), NCLVector3(1, 1, 1), false));
		}

		if (transmitMinionColour)
		{
			transmitMinionColour = false;
			broadcastPaintedMinion();
		}

	}
	else
	{
		waitingInLobbyText.addTextWhenTimeHasReachedMaximum(30);
		DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "deltatime disable"));

		DeliverySystem::getPostman()->insertMessage(TextMeshMessage("RenderingSystem", waitingInLobbyText.getCurrentString(),
			NCLVector3(-200, 0, 0), NCLVector3(20, 20, 1), NCLVector3(1, 1, 1), true));
	}

	timer->beginChildTimedSection("Process Network Messages");
	if (connectedToServer)
	{
		processNetworkMessages(deltaTime);
		DeliverySystem::getPostman()->insertMessage(TextMessage("Profiler", "Incoming K/Bs : " + std::to_string(network.m_IncomingKb)));
		DeliverySystem::getPostman()->insertMessage(TextMessage("Profiler", "Outgoing K/Bs : " + std::to_string(network.m_OutgoingKb)));
	}
	timer->endChildTimedSection("Process Network Messages");

	timer->endTimedSection();
}

void NetworkClient::connectToServer()
{
	if (network.Initialize(0))
	{
		serverConnection = network.ConnectPeer(10, 70, 32, 168, 1234);
		connectedToServer = true;
	}
}

void NetworkClient::waitForOtherClients(int numberToWaitFor)
{
	numberOfOtherPlayersToWaitFor = numberToWaitFor;
	inLobby = true;
}

void NetworkClient::broadcastKinematicState()
{
	timeSinceLastBroadcast = 0.0f;

	const std::string playerName = "player" + to_string(clientID);
	GameObject* client = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(playerName));

	KinematicState state;
	state.clientID = clientID;
	state.position = client->getPhysicsNode()->getPosition();
	state.linearVelocity = client->getPhysicsNode()->getLinearVelocity();
	state.linearAcceleration = client->getPhysicsNode()->getAcceleration();

	ENetPacket* packet = enet_packet_create(&state, sizeof(KinematicState), 0);
	enet_peer_send(serverConnection, 0, packet);
}

void NetworkClient::broadcastMinionState()
{
	for (int i = 0; i < objectsToToTransmitStatesFor.size(); ++i)
	{
		MinionKinematicState state;
		state.minionIndex = i;
		state.position = objectsToToTransmitStatesFor[i]->getPosition();
		state.linearVelocity = objectsToToTransmitStatesFor[i]->getPhysicsNode()->getLinearVelocity();
		state.linearAcceleration = objectsToToTransmitStatesFor[i]->getPhysicsNode()->getAcceleration();

		ENetPacket* packet = enet_packet_create(&state, sizeof(MinionKinematicState), 0);
		enet_peer_send(serverConnection, 0, packet);
	}
}

void NetworkClient::broadcastPaintedMinion()
{
	ENetPacket* packet = enet_packet_create(&minionColour, sizeof(MinionColour), 0);
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

void NetworkClient::updateDeadReckoningForMinions()
{
	for (auto client = minionDeadReckonings.begin(); client != minionDeadReckonings.end(); ++client)
	{
		client->second.predictPosition(UPDATE_TIMESTEP);
		client->second.blendStates(client->first->getPhysicsNode());
	}
}

void NetworkClient::processNetworkMessages(const float& deltaTime)
{
	network.ServiceNetwork(deltaTime, [&serverConnection = serverConnection, &gameplay = gameplay,
		&keyboardAndMouse = keyboardAndMouse, &playerbase = playerbase, &clientID = clientID, &inLobby = inLobby,
		&joinedGame = joinedGame, &database = database, &numberOfOtherPlayersToWaitFor = numberOfOtherPlayersToWaitFor,
		&msCounter = msCounter, &clientDeadReckonings = clientDeadReckonings, &minionDeadReckonings = minionDeadReckonings,
		&objectsToToTransmitStatesFor = objectsToToTransmitStatesFor](const ENetEvent& evnt)
	{
		if (evnt.type == ENET_EVENT_TYPE_RECEIVE)
		{
			if (evnt.packet->dataLength == sizeof(IntegerData))
			{
				IntegerData message;
				memcpy(&message, evnt.packet->data, sizeof(IntegerData));

				if (message.type == NEW_ID)
				{
					clientID = message.data;
					PaintGameActionBuilder::localPlayer = "player" + to_string(clientID);
					NetworkMessageProcessor::joinGame(clientID, playerbase, gameplay, keyboardAndMouse);
					joinedGame = true;
				}
				else if (message.type == CURRENT_NUMBER_OF_PLAYERS)
				{
					if (message.data == numberOfOtherPlayersToWaitFor)
					{
						DeliverySystem::getPostman()->insertMessage(TextMessage("GameLoop", "deltatime enable"));
						inLobby = false;
					}
				}
			}
			else if (evnt.packet->dataLength == sizeof(KinematicState) && joinedGame)
			{
				KinematicState recievedState;
				memcpy(&recievedState, evnt.packet->data, sizeof(KinematicState));

				if (recievedState.clientID != clientID)
				{
					const std::string playerName = "player" + to_string(recievedState.clientID);

					GameObject* client = NetworkMessageProcessor::getUpdatedDeadReckoningGameObject(playerName,
						recievedState, database);

					clientDeadReckonings[client] = DeadReckoning(recievedState);
				}
			}
			else if (evnt.packet->dataLength == sizeof(MinionKinematicState) && joinedGame)
			{
				MinionKinematicState recievedState;
				memcpy(&recievedState, evnt.packet->data, sizeof(MinionKinematicState));

				if (clientID != 0)
				{

					GameObject* client = NetworkMessageProcessor::getUpdatedDeadReckoningGameObject(objectsToToTransmitStatesFor[recievedState.minionIndex]->getName(),
						recievedState, database);

					minionDeadReckonings[client] = MinionDeadReckoning(recievedState);
				}
			}
			else if (evnt.packet->dataLength == sizeof(MinionColour) && joinedGame)
			{
				MinionColour recievedState;
				memcpy(&recievedState, evnt.packet->data, sizeof(MinionColour));

				objectsToToTransmitStatesFor[recievedState.minionIndex]->stats.colourToPaint = recievedState.minionColour;
				objectsToToTransmitStatesFor[recievedState.minionIndex]->getSceneNode()->SetColour(recievedState.minionColour);
			}
		}

		enet_packet_destroy(evnt.packet);
	});
}
