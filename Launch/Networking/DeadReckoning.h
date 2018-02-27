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
	static KinematicState predictState(KinematicState oldState, float deltaTime);
	static KinematicState blendStates(KinematicState oldState, KinematicState newState, float blendFactor);
	static Vector3 predictLinearPosition(KinematicState& oldState, float deltaTime);
};

