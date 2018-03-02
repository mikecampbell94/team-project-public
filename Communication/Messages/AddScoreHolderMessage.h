#pragma once

#include "../Message.h"
#include "../Utilities/Maths/Vector4.h"

struct Node;

class AddScoreHolderMessage : public Message
{
public:
	AddScoreHolderMessage(const std::string& desinationName, const std::string& name);
	~AddScoreHolderMessage();

	static AddScoreHolderMessage builder(Node* node);

	std::string name;
};

