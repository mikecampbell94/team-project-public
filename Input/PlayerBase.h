#pragma once
#include "Player.h"
#include <map>
#include <vector>

class PlayerBase
{
public:
	PlayerBase();
	PlayerBase(std::vector<InputRecorder*> allRecorders);
	~PlayerBase();



	void initializePlayers(std::vector<InputRecorder*> allRecorders);

	Player* addNewPlayer(InputRecorder* recorder);
	void removePlayer(int playerID);
	void removePlayer(Player* playerRef);



	std::map<Player*, InputRecorder*>& getPlayerInputMap()
	{
		return connectedPlayers;
	}

	std::vector<Player*>& getPlayers()
	{
		return players;
	}



private:
	std::vector<Player*> players;
	std::map<Player*, InputRecorder*> connectedPlayers;


	int generateNewID();
};