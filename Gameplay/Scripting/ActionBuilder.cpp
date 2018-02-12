#include "ActionBuilder.h"

#include "../Resource Management/XMLParser.h"
#include "../Communication/Message.h"
#include "../../Communication/MessagingService.h"
#include "../../Communication/DeliverySystem.h"

const std::string CONDITION_TYPE = "Condition";
const std::string SEND_MESSAGE_TYPE = "SendMessage";

GameplayAction ActionBuilder::buildAction(Node* node)
{
	bool containsCondition = false;
	Condition condition;
	std::vector<Executable> executables;

	for each (Node* section in node->children)
	{
		if (section->nodeType == CONDITION_TYPE)
		{
			condition = [section](Message message)
			{
				return message.getDataField(section->children[0]->nodeType) == section->children[0]->value;
			};

			containsCondition = true;
		}
		else
		{
			if (section->nodeType == SEND_MESSAGE_TYPE)
			{
				executables.push_back(buildSendMessageExecutable(section));
			}
		}
	}

	if (containsCondition)
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
	else
	{
		return [executables](Message message)
		{
			for each (Executable executable in executables)
			{
				executable();
			}
		};
	}
}

Executable ActionBuilder::buildSendMessageExecutable(Node* node)
{
	if (node->name == "DUMMY_TYPE")
	{
		return [node]()
		{
			//DeliverySystem::getPostman()->insertMessage(Message(node->children[0]->value, DUMMY_TYPE));
			DeliverySystem::getPostman()->insertMessage(Message("RenderingSystem", DUMMY_TYPE));
		};
	}
}
