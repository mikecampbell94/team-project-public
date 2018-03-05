#include "ActionBuilder.h"

#include "ConditionalStatementBuilder.h"
#include "../Resource Management/XMLParser.h"
#include "../Communication/Message.h"
#include "../../Communication/MessagingService.h"
#include "../../Communication/DeliverySystem.h"
#include "../../Communication/Messages/TextMessage.h"
#include "../../Communication/Messages/RelativeTransformMessage.h"
#include "../../Communication/Messages/MoveCameraRelativeToGameObjectMessage.h"
#include "../../Communication/Messages/PreparePaintSurfaceMessage.h"
#include "../../Communication/Messages/PaintTrailForGameObjectMessage.h"
#include "../../Communication/Messages/AddScoreHolderMessage.h"
#include "../../Communication/SendMessageActionBuilder.h"

const std::string CONDITIONAL_STATEMENT = "Condition";
const std::string SEND_MESSAGE_STATEMENT = "SendMessage";

GameplayAction ActionBuilder::buildAction(Node* node)
{
	Condition condition;
	std::vector<Executable> executables;

	for each (Node* section in node->children)
	{
		compileActionSection(section, condition, executables);
	}

	if (condition)
	{
		return buildFinalActionWithCondition(condition, executables);
	}
	else
	{
		buildFinalAction(executables);
	}
}

TimedGameplayAction ActionBuilder::buildTimedAction(Node* node)
{
	std::vector<Executable> executables;

	for each (Node* section in node->children)
	{
		executables.push_back(compileActionSectionWithoutCondition(section));
	}

	float interval = std::stof(node->name);

	return [interval, executables](float& timer)
	{
		if (timer >= interval)
		{
			timer = 0.0f;

			for each (Executable executable in executables)
			{
				executable();
			}
		}
	};
}

GameplayAction ActionBuilder::buildFinalActionWithCondition(Condition& condition, std::vector<Executable>& executables)
{
	return [condition, executables](Message message)
	{
		if (condition(message))
		{
			for each (Executable executable in executables)
			{
				executable();
			}
		}
	};
}

GameplayAction ActionBuilder::buildFinalAction(std::vector<Executable>& executables)
{
	return [executables](Message message)
	{
		for each (Executable executable in executables)
		{
			executable();
		}
	};
}

void ActionBuilder::compileActionSection(Node* section, Condition& condition, std::vector<Executable>& executables)
{
	if (section->nodeType == CONDITIONAL_STATEMENT)
	{
		condition = buildIfStatement(section);
	}
	else
	{
		executables.push_back(compileActionSectionWithoutCondition(section));
	}
}

Executable ActionBuilder::compileActionSectionWithoutCondition(Node* section)
{
	if (section->nodeType == SEND_MESSAGE_STATEMENT)
	{
		return buildSendMessageExecutable(section);
	}
	else
	{
		//Lookup the function in a map and return
	}
}

Condition ActionBuilder::buildIfStatement(Node* node)
{
	if (node->name == "OR")
	{
		return ConditionalStatementBuilder::buildOrCondition(node);
	}
	else if (node->name == "AND")
	{
		return ConditionalStatementBuilder::buildAndCondition(node);
	}
	else
	{
		return ConditionalStatementBuilder::buildSingleIfCondition(node);
	}
}

Executable ActionBuilder::buildSendMessageExecutable(Node* node)
{
	return SendMessageActionBuilder::buildSendMessageAction(node);
}
