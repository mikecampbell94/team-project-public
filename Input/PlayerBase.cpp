#include "PlayerBase.h"

//remove these later
std::vector<InputRecorder*> getAllConnectedControllers() {};


PlayerBase::PlayerBase() 
{

}

PlayerBase::PlayerBase(std::vector<InputRecorder*> allRecorders)
{
	initializePlayers(allRecorders);
}

PlayerBase::~PlayerBase()
{
	//delete all players
	if (!connectedPlayers.empty)
		for each (auto it in connectedPlayers)
		{
			delete it.second;
		}
}

void PlayerBase::initializePlayers(std::vector<InputRecorder*> allRecorders)
{
	if (!connectedPlayers.empty)
	{
		for each (auto it in connectedPlayers)
		{
			delete it.second;
		}

		connectedPlayers.clear();
	}
	
	for each (InputRecorder* i in allRecorders)
	{
		addNewPlayer(i);
	}
}

Player* PlayerBase::addNewPlayer(InputRecorder* recorder)
{
	int playerID = generateNewID();
	Player* playerRef = new Player(playerID, recorder);

	connectedPlayers.insert(std::pair<int,Player*>(playerID, playerRef));

	return playerRef;
}

void PlayerBase::removePlayer(int playerID)
{
	if (connectedPlayers.count(playerID) > 0)
	{
		delete connectedPlayers.at(playerID);
		connectedPlayers.erase(playerID);
	}
}

void PlayerBase::removePlayer(Player* playerRef) 
{
	for each (auto it in connectedPlayers)
	{
		if (it.second == playerRef)
		{
			delete it.second;
			connectedPlayers.erase(it.first);
			break;
		}
	}
}

//need optimalize
int PlayerBase::generateNewID() 
{
	if (connectedPlayers.empty())
	{
		return 0;
	}
	else
	{
		return (*connectedPlayers.rbegin()).first + 1;
	}
}