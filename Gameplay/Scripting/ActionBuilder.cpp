#include "ActionBuilder.h"

#include "ConditionalStatementBuilder.h"
#include "../Resource Management/XMLParser.h"
#include "../Communication/Message.h"
#include "../../Communication/MessagingService.h"
#include "../../Communication/DeliverySystem.h"

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
	else if (section->nodeType == SEND_MESSAGE_STATEMENT)
	{
		executables.push_back(buildSendMessageExecutable(section));
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
	if (node->name == "DUMMY_TYPE")
	{
		Node* destination = node->children[0];
		return [destination]()
		{
			DeliverySystem::getPostman()->insertMessage(Message(destination->value, DUMMY_TYPE));
		};
	}
}
