#include "InputManager.h"
#include "InputRecorder.h"

InputManager::InputManager(PlayerBase* playerbase, Window* window)
{
	this->window = window;
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

		playerbase->getPlayerInputMap().at(player)->clearInputs();
		playerbase->getPlayerInputMap().at(player)->fillInputs();
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