#include "TogglePlayerInputKeyMessage.h"

TogglePlayerInputKeyMessage::TogglePlayerInputKeyMessage(const std::string& destinationName,
	const std::string& playerGameObjectName, int key, bool enabled)
	: Message(destinationName, TOGGLE_PLAYER_INPUT)
{
	this->playerGameObjectName = playerGameObjectName;
	this->key = key;
	this->enabled = enabled;
}

TogglePlayerInputKeyMessage::~TogglePlayerInputKeyMessage()
{
}

TogglePlayerInputKeyMessage TogglePlayerInputKeyMessage::builder(Node* node)
{
	std::string nodeDestination = "";
	std::string nodePlayerGameObject = "";
	int key = 0;
	bool enabled = false;

	for (Node* childNode : node->children)
	{
		if (childNode->nodeType == "destination")
		{
			nodeDestination = childNode->value;
		}
		else if (childNode->nodeType == "player")
		{
			nodePlayerGameObject = childNode->value;
		}
		else if (childNode->nodeType == "key")
		{
			key = InputUtility::getKeyID(childNode->value);//stoi(childNode->value);
		}
		else if (childNode->nodeType == "enabled")
		{
			enabled = childNode->value == "True";
		}
	}

	return TogglePlayerInputKeyMessage(nodeDestination, nodePlayerGameObject, key, enabled);
}
