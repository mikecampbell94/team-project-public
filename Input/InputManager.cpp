#include "InputManager.h"
#include "InputRecorder.h"

InputManager::InputManager(PlayerBase* playerbase)
{
	this->playerbase = playerbase;
}

InputManager::~InputManager()
{
	delete playerbase;
}

//Fill the buffers and use them!
void InputManager::updateSubsystem(const float& deltatime)
{

	for (Player* player : playerbase->getPlayers())
	{
		std::cout << "-------PLAYER " << player->getPlayerID() << " INPUTS-------- \n";
		player->getInputRecorder()->clearInputs();
		player->getInputRecorder()->fillInputs();
	}
}


//void InputManager::Read(const string resourcename) - uncomment this once resource class is added
//{
//	this->SetName(resourcename);
//}

//void InputManager::ReadParams(const string params) - uncomment this once resource class is added
//{
//	Read(params);
//}