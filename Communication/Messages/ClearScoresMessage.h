#pragma once

#include "../Communication/Message.h"

struct Node;

class ClearScoresMessage : public Message
{
public:
	ClearScoresMessage(const std::string& destinationName, const std::string& moduleName);
	~ClearScoresMessage();

	std::string moduleName;

};

