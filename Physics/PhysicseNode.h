#pragma once
#include "../Utilities/Maths/Vector3.h"
class PhysicsNode
{
public:
	PhysicsNode();
	~PhysicsNode();


	Vector3 getPosition();
	void setPosition(Vector3 pos);

private:
	Vector3 position;
};

