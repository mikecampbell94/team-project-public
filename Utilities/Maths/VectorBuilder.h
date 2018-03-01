#pragma once
#include "../../Resource Management/XMLParser.h"

class VectorBuilder {
public:
	static float getVectorComponentFromNode(const Node* node)
	{
		if (node->nodeType == "random")
		{
			float min = std::stof(node->children[0]->value);
			float max = std::stof(node->children[1]->value);
			return getRandomVectorComponent(min, max);
		}
		else
		{
			return std::stof(node->value);
		}
	}
	static float getRandomVectorComponent(float mi, float ma)
	{
			return (rand() % (int)((ma - mi) * 1000)) / 1000 + mi;	
	}

private:
	float min;
	float max;
};