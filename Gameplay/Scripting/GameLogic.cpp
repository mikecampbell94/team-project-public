#include "GameLogic.h"

#include "../Communication/MessageProcessor.h"
#include "../Resource Management/XMLParser.h"
#include "ActionBuilder.h"

GameLogic::GameLogic(MessageProcessor* messages)
{
	this->messages = messages;
}

GameLogic::~GameLogic()
{
}

void GameLogic::compileParsedXMLIntoScript(Node* xmlNode)
{
	for each (Node* gameplayAction in xmlNode->children)
	{
		if (gameplayAction->nodeType == "ReceiveMessage")
		{
			for each (Node* action in gameplayAction->children)
			{
				messageBasedActions[gameplayAction->name].push_back(ActionBuilder::buildAction(action));
			}
		}
		else if (gameplayAction->nodeType == "Timed")
		{
			timedActions.push_back(ActionBuilder::buildTimedAction(gameplayAction));
		}
	}
}

void GameLogic::executeMessageBasedActions()
{
	for (int i = 0; i < publishers.size(); ++i)
	{
		std::vector<GameplayAction>* executables = &messageBasedActions.at(publishers[i].first);

		for each (GameplayAction executable in *executables)
		{
			executable(publishers[i].second);
		}
	}
}

void GameLogic::executeTimeBasedActions(const float& time, const float& deltaTime)
{
	for each (TimedGameplayAction timedAction in timedActions)
	{
		timedAction(time, deltaTime);
	}
}

void GameLogic::notifyMessageActions(const std::string& messageType, Message* message)
{
	publishers.push_back(std::make_pair(messageType, *message));
}

void GameLogic::clearNotifications()
{
	publishers.clear();
}
