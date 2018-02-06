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

	

private:
	std::map<int,Player*> connectedPlayers;

	int generateNewID();
};