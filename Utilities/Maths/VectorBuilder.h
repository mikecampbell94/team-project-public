#pragma once
#include "../../Resource Management/XMLParser.h"

class VectorBuilder {
public:
	static float getVectorComponentFromNode(const Node* node, float* min = nullptr, float* max = nullptr)
	{
		if (node->nodeType == "random")
		{

			*min = std::stof(node->children[0]->value);
			*max = std::stof(node->children[1]->value);
			return getRandomVectorComponent(*min, *max);
		}
		else
		{
			if(min != nullptr)
				*min = 0.f;
			if(max != nullptr)
				*max = 0.f;
			return std::stof(node->value);
		}
	}
	static float getRandomVectorComponent(const float mi, const float ma)
	{
			return (rand() % (int)((ma - mi) * 1000)) / 1000 + mi;	
	}
};