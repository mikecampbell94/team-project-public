#pragma once

#include <string>

class Player
{

public:
	
	Player();
	
	~Player();


	void setPlayerID(std::string newID)
	{
		playerID = newID;
	}

	const std::string getPlayerID()
	{
		return playerID;
	}


private:

	std::string playerID;

	


};

