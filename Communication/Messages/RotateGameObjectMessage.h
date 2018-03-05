#pragma once

#include "../Communication/Message.h"
#include "../Utilities/Maths/Vector4.h"
#include <string>

struct Node;

class RotateGameObjectMessage : public Message
{
public:
	RotateGameObjectMessage(const std::string& desinationName, std::string gameObjectID,
		NCLVector4 rotation);
	~RotateGameObjectMessage();

	static RotateGameObjectMessage builder(Node* node);
	static RotateGameObjectMessage tokensToMessage(std::vector<std::string> lineTokens);

	std::string gameObjectID;
	NCLVector4 rotation;
};

