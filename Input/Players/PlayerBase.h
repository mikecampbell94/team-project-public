#pragma once
#include "Player.h"
#include "../Resource Management/XMLParser.h"
#include "../Input/InputUtility.h"
#include "../../Gameplay/InputGameplay/InputActionMap.h"
#include <map>
#include <vector>

class Database;

class PlayerBase
{
public:
	PlayerBase(Database* database);
	PlayerBase(Database* database, std::vector<InputRecorder*> allRecorders);
	~PlayerBase();

	void initializePlayers(std::vector<InputRecorder*> allRecorders);

	Player* addNewPlayer(InputRecorder* recorder, int id);
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
	std::vector<InputActionMap> playersActions;

	XMLParser inputParser;
	Database* database;
};