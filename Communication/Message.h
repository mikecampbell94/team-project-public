#pragma once

enum DestinationSubsystem
{
	AUDIO,
	COMMUNICATION,
	GAMEPLAY,
	GRPAHICS,
	INPUT_SYSTEM,
	INTERFACE,
	LAUNCH,
	PHYSICS,
	PROFILER,
	RESOURCE_MANAGEMENT,
	UTILITIES
};

enum MessageType
{
	DUMMY_TYPE
};

class Message
{

public:
	Message(DestinationSubsystem destination, MessageType type);
	
	virtual ~Message() {}

	const DestinationSubsystem getDestination() const
	{ 
		return destination;
	}

	const MessageType getMessageType() const
	{ 
		return type; 
	}
	
protected:
	DestinationSubsystem destination;
	MessageType type;
};

