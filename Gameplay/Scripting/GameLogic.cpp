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
	if (xmlNode->nodeType == "ReceiveMessage")
	{
		for each (Node* action in xmlNode->children)
		{
			messageBasedActions[xmlNode->name].push_back(ActionBuilder::buildAction(action));
		}
	}
	else if (xmlNode->nodeType == "Timed")
	{
		
	}
}

void GameLogic::executeMessageBasedActions()
{
	//for each (std::pair<std::string, Message> publisher in publishers)
	for (int i = 0; i < publishers.size(); ++i)
	{
		std::vector<GameplayAction>* executables = &messageBasedActions.at(publishers[i].first);

		for each (GameplayAction executable in *executables)
		{
			executable(publishers[i].second);
		}
	}
}

void GameLogic::executeDeltaTimeBasedActions(const float& deltaTime)
{
}

void GameLogic::notifyMessageActions(const std::string& messageType, Message* message)
{
	publishers.push_back(std::make_pair(messageType, *message));
}

void GameLogic::clearNotifications()
{
	publishers.clear();
}
