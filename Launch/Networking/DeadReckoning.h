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
	float timeStamp;
};

class DeadReckoning
{
public:
	//static void predictPosition(PhysicsNode* node, float deltaTime);
	static void blendStates(PhysicsNode* node, KinematicState oldState, float factor);

private:
	static Vector3 interpolate(Vector3 a, Vector3 b, float factor);
};

