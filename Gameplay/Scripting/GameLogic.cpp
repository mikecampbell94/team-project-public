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
	for (Node* gameplayAction : xmlNode->children)
	{
		if (gameplayAction->nodeType == "ReceiveMessage")
		{
			for (Node* action : gameplayAction->children)
			{
				messageBasedActions[gameplayAction->name].push_back(ActionBuilder::buildAction(action));
			}
		}
		else if (gameplayAction->nodeType == "Timed")
		{
			timers.push_back(float(0.0f));
			timedActions.push_back(ActionBuilder::buildTimedAction(gameplayAction));
		}
		else if (gameplayAction->nodeType == "OnStart")
		{
			for (Node* action : gameplayAction->children)
			{
				actionsOnStart.push_back(ActionBuilder::compileActionSectionWithoutCondition(action));
			}
		}
	}
}

void GameLogic::executeMessageBasedActions()
{
	if(!messageBasedActions.empty())
	{
		for (int i = 0; i < publishers.size(); ++i)
		{
			//if (messageBasedActions.find(publishers[i].first) != messageBasedActions.end())
			//{
				std::vector<GameplayAction>* executables = &messageBasedActions.at(publishers[i].first);

				for (GameplayAction executable : *executables)
				{
					executable(publishers[i].second);
				}
			//}
		}
	}
}

void GameLogic::executeTimeBasedActions(const float& deltaTime)
{
	for (int i = 0; i < timedActions.size(); ++i)
	{
		timers[i] += deltaTime;
		timedActions[i](timers[i]);
	}
}

void GameLogic::executeActionsOnStart()
{
	for (Executable executable : actionsOnStart)
	{
		executable();
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
