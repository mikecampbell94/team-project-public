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

	incomingMessages.addActionToExecuteOnMessage(MessageType::TOGGLE_PLAYER_INPUT, [&keysToBlock = keysToBlock, &keysToUnblock = keysToUnblock](Message* message)
	{
		TogglePlayerInputKeyMessage* toggleInputMessage = static_cast<TogglePlayerInputKeyMessage*>(message);

		if (!toggleInputMessage->enabled)
		{
			keysToBlock.push_back(std::make_pair(toggleInputMessage->playerGameObjectName, toggleInputMessage->key));
		}
		else
		{
			keysToUnblock.push_back(std::make_pair(toggleInputMessage->playerGameObjectName, toggleInputMessage->key));
		}
	});

	blocked = false;
	this->playerbase = playerbase;
}

InputManager::~InputManager()
{
	delete playerbase;
}

void InputManager::updateSubsystem(const float& deltatime)
{
	blockKeysFromMessages();
	unblockKeysFromMessages();

	if (!blocked)
	{
		timer->beginTimedSection();

		for (Player* player : playerbase->getPlayers())
		{
			player->getInputRecorder()->clearInputs();
			player->getInputRecorder()->fillInputs();

			std::vector<ButtonInputData> inputData = player->getInputRecorder()->getInputs();

			sendInputMessageForUnblockedKeys(inputData, player);
		}

		timer->endTimedSection();
	}
}

void InputManager::sendInputMessageForUnblockedKeys(std::vector<ButtonInputData>& inputData, Player* player)
{
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

void InputManager::blockKeysFromMessages()
{
	for (std::pair<std::string, int> keyToBlock : keysToBlock)
	{
		bool alreadyBlocked = false;

		for (auto blockedKeysIterator = blockedKeysForEachPlayer.begin(); blockedKeysIterator != blockedKeysForEachPlayer.end(); ++blockedKeysIterator)
		{
			if ((*blockedKeysIterator).first == keyToBlock.first && (*blockedKeysIterator).second == keyToBlock.second)
			{
				alreadyBlocked = true;
				break;
			}
		}

		if (!alreadyBlocked)
		{
			blockedKeysForEachPlayer.push_back(std::make_pair(keyToBlock.first, keyToBlock.second));
		}
	}

	keysToBlock.clear();
}

void InputManager::unblockKeysFromMessages()
{
	for (std::pair<std::string, int> keyToUnblock : keysToUnblock)
	{
		for (auto blockedKeysIterator = blockedKeysForEachPlayer.begin(); blockedKeysIterator != blockedKeysForEachPlayer.end(); ++blockedKeysIterator)
		{
			if ((*blockedKeysIterator).first == keyToUnblock.first && (*blockedKeysIterator).second == keyToUnblock.second)
			{
				blockedKeysForEachPlayer.erase(blockedKeysIterator);
				break;
			}
		}
	}

	keysToUnblock.clear();
}