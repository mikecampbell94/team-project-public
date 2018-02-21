#pragma once
#include "../Communication/Message.h"
#include <string>
#include "../Utilities/Maths/Vector3.h"

class ApplyForceMessage : public Message
{
public:
	ApplyForceMessage(const std::string& desinationName, std::string gameObjectID, Vector3 force);
	~ApplyForceMessage();

	std::string gameObjectID;
	Vector3 force;


};

