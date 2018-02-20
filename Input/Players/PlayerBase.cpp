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


	InputActionMap newPlayersActions(playerID);
	inputParser.loadFile("../Data/Resources/Input/configXML.xml");
	Node* node = inputParser.parsedXml;

	//NEED TO FUCK ABOUT WITH THIS IN ORDER TO ADD ANYTHING OTHER THAN WASD MOVEMENT
	for (int i = 0; i < inputParser.parsedXml->children.size(); i++) {
		newPlayersActions.attachKeyToAction(InputUtility::getKeyID(inputParser.parsedXml->children[i]->children[0]->value), [coordinate = node->children[i]->children[1]->children[0], i](Player* player)
		{
			float xPosition = stof(coordinate->children[0]->value);
			float yPosition = stof(coordinate->children[1]->value);
			float zPosition = stof(coordinate->children[2]->value);

			Vector3 translation(xPosition, yPosition, zPosition);

			player->getSceneNode()->SetTransform(player->getSceneNode()->GetTransform() * Matrix4::translation(translation));
		});
	}
	playersActions.push_back(newPlayersActions);
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
