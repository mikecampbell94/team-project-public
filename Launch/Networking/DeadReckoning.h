#pragma once
#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Quaternion.h"

class PhysicsNode;

struct KinematicState
{
	int id;

	Vector3 position;
	Vector3 linearVelocity;
	Vector3 linearAcceleration;
};

class DeadReckoning
{
public:
	static void predictPosition(PhysicsNode* node, float deltaTime);
};

