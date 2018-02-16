#include "ConditionalStatementBuilder.h"

#include "../Communication/Message.h"
#include "../Resource Management/XMLParser.h"

Condition ConditionalStatementBuilder::buildOrCondition(Node* node)
{
	return [node](Message message)
	{
		bool condition = false;

		for each (Node* childCondition in node->children)
		{
			condition = condition || message.getDataField(childCondition->nodeType) == childCondition->value;
		}

		return condition;
	};
}

Condition ConditionalStatementBuilder::buildAndCondition(Node* node)
{
	return [node](Message message)
	{
		bool condition = true;

		for each (Node* childCondition in node->children)
		{
			condition = condition && message.getDataField(childCondition->nodeType) == childCondition->value;
		}

		return condition;
	};
}

Condition ConditionalStatementBuilder::buildSingleIfCondition(Node* node)
{
	Node* conditionNode = node->children[0];

	return [conditionNode](Message message)
	{
		return message.getDataField(conditionNode->nodeType) == conditionNode->value;
	};
}
