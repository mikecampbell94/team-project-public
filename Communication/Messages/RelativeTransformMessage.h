#pragma once

#include "../Message.h"
#include "../Utilities/Maths/Matrix4.h"

struct Node;

class RelativeTransformMessage : public Message
{
public:
	RelativeTransformMessage(const std::string& destinationName, const std::string& resourceName, 
		Matrix4 transform);
	~RelativeTransformMessage();

	static RelativeTransformMessage builder(Node* node);

	Matrix4 transform;
	std::string resourceName;
};

