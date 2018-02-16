#pragma once

#include <string>

enum MessageType
{
	PLAYER_INPUT,
	DUMMY_TYPE,
	PLAY_SOUND,
	STOP_SOUND,
	MOVING_SOUND
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
	
protected:
	std::string destination;
	MessageType type;
};

