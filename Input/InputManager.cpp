#include "InputManager.h"

#include "Recorders/InputRecorder.h"
#include "../Communication/DeliverySystem.h"
#include "Communication/Messages/PlayerInputMessage.h"
#include "Communication/LetterBox.h"
#include "../Gameplay/GameObject.h"
#include "../Physics/PhysicsNode.h"
#include <iterator>
#include "Communication/Messages/TogglePlayerInputKeyMessage.h"

InputManager::InputManager(PlayerBase* playerbase)
	: Subsystem("InputManager")
{
	incomingMessages = MessageProcessor(std::vector<MessageType> { MessageType::TEXT, TOGGLE_PLAYER_INPUT },
		DeliverySystem::getPostman()->getDeliveryPoint("InputManager"));

	inputControl.registerNewInputUserByDeliveryPoint("InputManager");

	incomingMessages.addActionToExecuteOnMessage(MessageType::TEXT, [&inputControl = inputControl, &blocked = blocked](Message* message)
	{
		TextMessage* textMessage = static_cast<TextMessage*>(message);

		istringstream iss(textMessage->text);
		vector<string> tokens{ istream_iterator<string>{iss},
			std::istream_iterator<string>{} };

		if (tokens[0] == "RegisterInputUser")
		{
			inputControl.registerNewInputUserByDeliveryPoint(tokens[1]);
		}
		else if (tokens[0] == "BlockAllInputs")
		{
			inputControl.blockAllInputUsersOtherThanCaller(tokens[1]);
		}
		else if (tokens[0] == "UnblockAll")
		{
			inputControl.unlockBlockedUsers();
		}
		else
		{
			blocked = InputControl::isBlocked(textMessage->text);
		}
	});

	incomingMessages.addActionToExecuteOnMessage(MessageType::TOGGLE_PLAYER_INPUT, [&blockedKeysForEachPlayer = blockedKeysForEachPlayer](Message* message)
	{
		TogglePlayerInputKeyMessage* toggleInputMessage = static_cast<TogglePlayerInputKeyMessage*>(message);

		//bool alreadyBlocked = false;
		//for (auto blockedKeysIterator = blockedKeysForEachPlayer.begin(); blockedKeysIterator != blockedKeysForEachPlayer.end();)
		//{
		//	if ((*blockedKeysIterator).first == toggleInputMessage->playerGameObjectName)
		//	{
		//		if ((*blockedKeysIterator).second == toggleInputMessage->key)
		//		{
		//			blockedKeysForEachPlayer.erase(blockedKeysIterator);
		//			alreadyBlocked = true;
		//			break;
		//		}
		//	}
		//}

		//if (!alreadyBlocked)
		//{
		//	blockedKeysForEachPlayer.push_back(std::make_pair(toggleInputMessage->playerGameObjectName, toggleInputMessage->key));
		//}

		if (!toggleInputMessage->enabled)
		{
			//check if already exists, if so, dont do anything
			//else insert

			bool alreadyBlocked = false;
 			for (auto blockedKeysIterator = blockedKeysForEachPlayer.begin(); blockedKeysIterator != blockedKeysForEachPlayer.end();)
			{
				if ((*blockedKeysIterator).first == toggleInputMessage->playerGameObjectName && (*blockedKeysIterator).second == toggleInputMessage->key)
				{
					//blockedKeysIterator = blockedKeysForEachPlayer.erase(blockedKeysIterator);
					alreadyBlocked = true;
				}
				//else
				//{
				//	++blockedKeysIterator;
				//}
			}

			if (!alreadyBlocked)
			{
				blockedKeysForEachPlayer.push_back(std::make_pair(toggleInputMessage->playerGameObjectName, toggleInputMessage->key));
			}
		}
		else
		{
			//find it and remove
		}
	});

	blocked = false;
	this->playerbase = playerbase;
}

InputManager::~InputManager()
{
	delete playerbase;
}

//Fill the buffers and use them!
void InputManager::updateSubsystem(const float& deltatime)
{
	if (!blocked)
	{
		timer->beginTimedSection();

		for (Player* player : playerbase->getPlayers())
		{
			player->getInputRecorder()->clearInputs();
			player->getInputRecorder()->fillInputs();

			std::vector<ButtonInputData> inputData = player->getInputRecorder()->getInputs();

			for (ButtonInputData singleInput : inputData)
			{
				bool blocked = false;

				for (std::pair<std::string, int> blockedInput : blockedKeysForEachPlayer)
				{
					if (blockedInput.first == player->getGameObject()->getName() && blockedInput.second == singleInput.key)
					{
						blocked = true;
					}
				}

				if (!blocked)
				{
					DeliverySystem::getPostman()->insertMessage(PlayerInputMessage("Gameplay", player, singleInput));
				}
			}
		}

		timer->endTimedSection();
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