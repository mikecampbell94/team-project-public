#pragma once

#include "../Communication/Message.h"
#include <string>
#include "../Utilities/Maths/Vector3.h"

class ApplyImpulseMessage : public Message
{
public:
	ApplyImpulseMessage(const std::string& desinationName, std::string gameObjectID, Vector3 impulse);
	~ApplyImpulseMessage();

	static ApplyImpulseMessage builder(Node* node);

	std::string gameObjectID;
	Vector3 impulse;
};

