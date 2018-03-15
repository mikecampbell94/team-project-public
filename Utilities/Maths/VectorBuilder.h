#pragma once
#include "../../Resource Management/XMLParser.h"
#include <random>

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
			{
				*min = 0.0f;
			}
			if(max != nullptr)
			{
				*max = 0.0f;
			}
			return std::stof(node->value);
		}
	}
	static float getRandomVectorComponent(const float mi, const float ma)
	{
		std::random_device rd;
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(mi, ma); // guaranteed unbiased
		auto r1 = uni(rng);
		return r1;	
	}
};