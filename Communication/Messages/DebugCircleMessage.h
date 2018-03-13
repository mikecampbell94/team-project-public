#pragma once
#include "../Communication/Message.h"
#include <string>
#include "../Utilities/Maths/Vector3.h"

class DebugCircleMessage : public Message
{
public:
	DebugCircleMessage(const std::string &destinationName, NCLVector3 position,
		float radius, NCLVector3 colour);
	~DebugCircleMessage();

	NCLVector3 position;
	NCLVector3 colour;
	float radius;
};

