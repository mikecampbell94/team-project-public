#pragma once

#include "../Message.h"
#include "../Utilities/Maths/Vector3.h"

class UpdatePositionMessage : public Message
{
public:
	UpdatePositionMessage(const std::string& desinationName, const std::string& gameObjectID, const Vector3& position);
	~UpdatePositionMessage();

	std::string gameObjectID;
	Vector3 position;
};

