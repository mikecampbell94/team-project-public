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

class Vector4	{
public:
	Vector4(void) {
		x = y = z = w = 1.0f;
	}
	Vector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector3 toVector3() {
		return Vector3(x, y, z);
	}

	~Vector4(void){}

	float x;
	float y;
	float z;
	float w;

	inline Vector4  operator-(const Vector4  &a) const
	{
		return Vector4(x - a.x, y - a.y, z - a.z, w - a.w);
	}

	static Vector4 builder(Node* node)
	{
		const float x = std::stof(node->children[0]->value);
		const float y = std::stof(node->children[1]->value);
		const float z = std::stof(node->children[2]->value);
		const float w = std::stof(node->children[3]->value);
		return Vector4(x, y, z, w);
	}
};

