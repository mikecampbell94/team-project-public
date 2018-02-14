#include "PlayerBase.h"

PlayerBase::PlayerBase()
{

}

PlayerBase::PlayerBase(std::vector<InputRecorder*> allRecorders)
{
	initializePlayers(allRecorders);
}

PlayerBase::~PlayerBase()
{
	wipeStoredPlayers();
}

void PlayerBase::initializePlayers(std::vector<InputRecorder*> allRecorders)
{
	wipeStoredPlayers();

	for each (InputRecorder* recorder in allRecorders)
	{
		addNewPlayer(recorder);
	}
}

Player* PlayerBase::addNewPlayer(InputRecorder* recorder)
{
	int playerID = players.size();
	Player* playerRef = new Player(playerID, recorder);
	players.push_back(playerRef);

	return playerRef;
}

void PlayerBase::removePlayer(int playerID)
{
	for (unsigned int i = 0; i < players.size(); ++i)
	{
		if (players[i]->getPlayerID() == playerID)
		{
			delete players[i];
			players.erase(players.begin() + i);
		}
	}
}

void PlayerBase::removePlayer(Player* playerRef)
{
	//todo 
}

void PlayerBase::wipeStoredPlayers()
{
	for each (auto player in players)
	{
		delete player;
	}

	players.clear();
}
