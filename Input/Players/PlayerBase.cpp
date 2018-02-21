#include "PlayerBase.h"

#include "../Recorders/InputRecorder.h"
#include "../../Resource Management/Database/Database.h"
#include "../../Gameplay/GameObject.h"
#include "../../Physics/PhysicsNode.h"

PlayerBase::PlayerBase(Database* database)
{
	this->database = database;
}

PlayerBase::PlayerBase(Database* database, std::vector<InputRecorder*> allRecorders)
{
	this->database = database;
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
	Player* player = new Player(playerID, recorder);
	players.push_back(player);

	InputActionMap newPlayersActions(playerID);
	inputParser.loadFile("../Data/Resources/Input/configXML.xml");
	Node* node = inputParser.parsedXml;


	//NOW ATTACH NODE
	GameObject* playerGameObject = static_cast<GameObject*>(database->getTable("GameObjects")->getResource("playerBall"));
	player->setGameObject(playerGameObject);

	std::string seperator = "|";
	std::string keyboardButtonsToListenTo = "";

	for (int i = 0; i < inputParser.parsedXml->children.size(); i++) 
	{
		std::string keyName = inputParser.parsedXml->children[i]->children[0]->value;

		if (i != inputParser.parsedXml->children.size() - 1)
		{
			keyboardButtonsToListenTo += keyName + seperator;
		}

		Node* magnitude = node->children[i]->children[1]->children[0];

		newPlayersActions.attachKeyToAction(InputUtility::getKeyID(keyName), [magnitude = magnitude](Player* player)
		{
			float xPosition = stof(magnitude->children[0]->value);
			float yPosition = stof(magnitude->children[1]->value);
			float zPosition = stof(magnitude->children[2]->value);

			Vector3 translation(xPosition, yPosition, zPosition);
			player->getGameObject()->getPhysicsNode()->setForce(Vector3(translation));
		});
	}

	std::vector<int> keyboardMouseConfiguration = player->getInputFilter()->getListenedKeys(keyboardButtonsToListenTo, seperator);
	player->getInputRecorder()->addKeysToListen(keyboardMouseConfiguration);

	playersActions.push_back(newPlayersActions);
	return player;
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
