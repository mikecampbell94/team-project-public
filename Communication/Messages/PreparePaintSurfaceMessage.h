#pragma once

#include "../Message.h"

struct Node;
class GameObject;

class PreparePaintSurfaceMessage : public Message
{
public:
	PreparePaintSurfaceMessage(const std::string& desinationName, std::vector<std::string> surfaceObjectIdentifiers);
	~PreparePaintSurfaceMessage();

	static PreparePaintSurfaceMessage builder(Node* node);

	std::vector<std::string> surfaceObjectIdentifiers;
};

