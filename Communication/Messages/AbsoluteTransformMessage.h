#pragma once

#include "../Message.h"
#include "../Utilities/Maths/Matrix4.h"

struct Node;

class AbsoluteTransformMessage : public Message
{
public:
	AbsoluteTransformMessage(const std::string& destinationName, const std::string& resourceName,
		Matrix4 transform);
	~AbsoluteTransformMessage();

	static AbsoluteTransformMessage builder(Node* node);
	static AbsoluteTransformMessage tokensToMessage(std::vector<std::string> lineTokens);

	Matrix4 transform;
	std::string resourceName;
};

