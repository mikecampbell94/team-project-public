#include "DebugCircleMessage.h"


DebugCircleMessage::DebugCircleMessage(const std::string & destinationName, NCLVector3 position,
	float radius, NCLVector3 colour) : Message(destinationName, DEBUG_CIRCLE)
{
	this->destination = destinationName;
	this->position = position;
	this->radius = radius;
	this->colour = colour;
}

DebugCircleMessage::~DebugCircleMessage()
{
}
