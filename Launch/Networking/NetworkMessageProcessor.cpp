#include "NetworkMessageProcessor.h"

#include "../../Resource Management/Database/Database.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"

bool NetworkMessageProcessor::isJoinGameMessage(ENetPacket* packet)
{
	return packet->dataLength == sizeof(int);
}

bool NetworkMessageProcessor::isKinematicStateMessage(ENetPacket* packet)
{
	return packet->dataLength == sizeof(KinematicState);
}

int NetworkMessageProcessor::getClientNumber(ENetPacket* joinGamePacket)
{
	int clientID;
	memcpy(&clientID, joinGamePacket->data, sizeof(int));

	return clientID;
}

void NetworkMessageProcessor::joinGame(int clientID, PlayerBase* playerbase, GameplaySystem* game,
	InputRecorder* playerInputRecorder)
{
	playerbase->addNewPlayer(playerInputRecorder, clientID);
	game->connectPlayerbase(playerbase);
}

GameObject* NetworkMessageProcessor::getUpdatedDeadReckoningGameObject(std::string objectName,
	KinematicState& kinematicState, Database* database)
{
	GameObject* client = static_cast<GameObject*>(database->getTable("GameObjects")->getResource(objectName));

	client->getPhysicsNode()->constantAcceleration = true;
	client->getPhysicsNode()->setLinearVelocity(kinematicState.linearVelocity);
	client->getPhysicsNode()->setAcceleration(kinematicState.linearAcceleration);

	return client;
}
