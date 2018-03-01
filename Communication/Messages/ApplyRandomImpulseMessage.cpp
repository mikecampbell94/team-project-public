#include "ApplyRandomImpulseMessage.h"

ApplyRandomImpulseMessage::ApplyRandomImpulseMessage(const std::string& desinationName, std::string gameObjectID, float xmin, float xman, float ymin, float ymax, float zmin, float zmax) : Message(desinationName, APPLY_RANDOM_IMPULSE)
{
	this->gameObjectID = gameObjectID;
	this->xmin = xmin;
	this->xmax = xmax;
}

ApplyRandomImpulseMessage::~ApplyRandomImpulseMessage()
{
}
