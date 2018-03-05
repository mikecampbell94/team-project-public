/*
Class:Vector4
Implements:
Author:Rich Davison
Description:VERY simple Vector4 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#pragma once

#include "Vector3.h"

class NCLVector4	{
public:
	NCLVector4(void) {
		x = y = z = w = 1.0f;
	}
	NCLVector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	NCLVector3 toVector3() {
		return NCLVector3(x, y, z);
	}

	~NCLVector4(void){}

	float x;
	float y;
	float z;
	float w;

	inline NCLVector4  operator-(const NCLVector4  &a) const
	{
		return NCLVector4(x - a.x, y - a.y, z - a.z, w - a.w);
	}

	static NCLVector4 builder(Node* node)
	{
		const float x = VectorBuilder::getVectorComponentFromNode(node->children[0]);
		const float y = VectorBuilder::getVectorComponentFromNode(node->children[1]);
		const float z = VectorBuilder::getVectorComponentFromNode(node->children[2]);
		const float w = VectorBuilder::getVectorComponentFromNode(node->children[3]);
		return NCLVector4(x, y, z, w);
	}

	static NCLVector4 builder(std::string text)
	{
		std::stringstream ss(text);
		std::string item;
		std::vector<std::string> tokens;

		while (std::getline(ss, item, ','))
		{
			tokens.push_back(item);
		}

		return NCLVector4(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
	}
};

