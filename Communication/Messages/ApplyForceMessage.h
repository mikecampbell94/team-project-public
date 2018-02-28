#pragma once
#include "../Communication/Message.h"
#include <string>
#include "../Utilities/Maths/Vector3.h"

class ApplyForceMessage : public Message
{
public:
	ApplyForceMessage(const std::string& desinationName, const std::string& gameObjectID, const Vector3& force);
	~ApplyForceMessage();

	static ApplyForceMessage builder(Node* node);

	std::string gameObjectID;
	Vector3 force;
};

