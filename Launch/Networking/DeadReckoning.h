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
	explicit DeadReckoning(KinematicState prediction);
	DeadReckoning() {}
	~DeadReckoning() {}

	void blendStates(PhysicsNode* node);
	void predictPosition(float deltaTime);

private:
	KinematicState prediction;
	float calculateInterpolationFactor(const Vector3& originalPosition);
};

