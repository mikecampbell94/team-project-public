#pragma once
#include "../Communication/Message.h"
#include <string>
#include "../Utilities/Maths/Vector3.h"

class DebugMessage : public Message
{
public:
	DebugMessage(const std::string &destinationName, NCLVector3 from, NCLVector3 to, NCLVector3 colour);
	~DebugMessage();

	NCLVector3 from;
	NCLVector3 to;
	NCLVector3 colour;
};

