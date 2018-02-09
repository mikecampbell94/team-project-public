#include "Message.h"

Message::Message(const std::string& destinationName,  MessageType type)
{
	this->destination = destinationName;
	this->type = type;
}

