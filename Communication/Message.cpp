#include "Message.h"

Message::Message(DestinationSubsystem destination,  MessageType type)
{
	this->destination = destination;
	this->type = type;
}

