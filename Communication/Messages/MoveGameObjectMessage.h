#pragma once

#include "../Communication/Message.h"
#include "../Utilities/Maths/Vector3.h"
#include <string>

struct Node;

class MoveGameObjectMessage : public Message
{
public:
	MoveGameObjectMessage(const std::string& desinationName, std::string gameObjectID,
		Vector3 position);
	~MoveGameObjectMessage();

	static MoveGameObjectMessage builder(Node* node);
	static MoveGameObjectMessage tokensToMessage(std::vector<std::string> lineTokens);

	std::string gameObjectID;
	Vector3 position;
};

