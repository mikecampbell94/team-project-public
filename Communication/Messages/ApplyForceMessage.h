#pragma once
#include "../Communication/Message.h"
#include <string>
#include "../Utilities/Maths/Vector3.h"

class ApplyForceMessage : public Message
{
public:
	ApplyForceMessage(const std::string& desinationName, const std::string& gameObjectID, bool isRandom,
		const Vector3 force, float xmin = 0.0f, float xmax = 0.0f, float ymin = 0.0f, float ymax = 0.0f,
		float zmin = 0.0f, float zmax = 0.0f);
	~ApplyForceMessage();

	static ApplyForceMessage builder(Node* node);

	std::string gameObjectID;
	bool isRandom;
	Vector3 force;

	float xmin, xmax;
	float ymin, ymax;
	float zmin, zmax;
};

