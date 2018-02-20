#pragma once
#include "Player.h"
#include "../Resource Management/XMLParser.h"
#include "../Input/InputUtility.h"
#include "../../Gameplay/InputGameplay/InputActionMap.h"
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

	std::vector<Player*>& getPlayers()
	{
		return players;
	}

	std::vector<InputActionMap> & getPlayersAction()
	{
		return playersActions;
	}



private:
	void wipeStoredPlayers();

	std::vector<InputRecorder*> inputRecorders;
	std::vector<Player*> players;
	XMLParser inputParser;
	std::vector<InputActionMap> playersActions;
};