#pragma once

enum DestinationSubsystem
{
	AUDIO,
	COMMUNICATION,
	GAMEPLAY,
	GRPAHICS,
	INPUT,
	INTERFACE,
	LAUNCH,
	PHYSICS,
	PROFILER,
	RESOURCE_MANAGEMENT,
	UTILITIES
};

enum MessageType
{
	//List to be determined...
	//e.g. AUDIO_PLAY_SOUND, etc
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

