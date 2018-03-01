#pragma once

#include "../Communication/Message.h"
#include <string>
#include "../Utilities/Maths/Vector3.h"

class ApplyRandomImpulseMessage : public Message
{
public:
	ApplyRandomImpulseMessage(const std::string& desinationName, std::string gameObjectID, float xmin, float xman, float ymin, float ymax, float zmin, float zmax);
	~ApplyRandomImpulseMessage();

	static ApplyRandomImpulseMessage builder(Node* node);

	std::string gameObjectID;
	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;

};

