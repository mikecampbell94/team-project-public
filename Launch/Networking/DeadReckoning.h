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
	DeadReckoning() {}
	~DeadReckoning() {}

	void blendStates(PhysicsNode* node, float factor);
	void predictPosition(float deltaTime);

	KinematicState prediction;
private:
	Vector3 interpolate(Vector3 a, Vector3 b, float factor);
};

