#pragma once

#include <string>
#include <unordered_map>

enum MessageType
{
	PLAYER_INPUT,
	DUMMY_TYPE
};

class Message
{

public:
	Message(const std::string& destinationName, MessageType type);
	
	virtual ~Message() {}

	const std::string getDestination() const
	{ 
		return destination;
	}

	const MessageType getMessageType() const
	{ 
		return type; 
	}

	std::string getDataField(std::string name)
	{
		if (name == "destination")
		{
			return destination;
		}
		else if (name == "type")
		{
			return messageTypeData.at(type);
		}
		else if (name == "playerID")
		{
			return integerInformation.at(name);
		}
	}
	
	void addIntegerInformation(std::string key, std::string value)
	{
		integerInformation.insert({ key, value });
	}

protected:
	std::string destination;
	MessageType type;

	std::unordered_map<int, std::string> messageTypeData;
	std::unordered_map<std::string, std::string> integerInformation;
};

