#include "DebugMessage.h"



DebugMessage::DebugMessage(const std::string &destinationName, NCLVector3 from, NCLVector3 to, NCLVector3 colour) : Message(destinationName, DEBUG)
{
	this->destination = destinationName;
	this->from = from;
	this->to = to;
	this->colour = colour;
}


DebugMessage::~DebugMessage()
{
}
