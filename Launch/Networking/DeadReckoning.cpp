#include "DeadReckoning.h"
#include "../Physics/PhysicsNode.h"

KinematicState DeadReckoning::predictState(KinematicState oldState, float deltaTime)
{
	KinematicState newState = oldState;
	
	newState.position = predictLinearPosition(oldState, deltaTime);
}

KinematicState DeadReckoning::blendStates(KinematicState oldState, KinematicState newState, float blendFactor)
{
}

Vector3 DeadReckoning::predictLinearPosition(KinematicState& oldState, float deltaTime)
{
	Vector3 position = oldState.position + (oldState.linearVelocity * deltaTime)
		+ ((oldState.linearAcceleration * 0.5f) * (deltaTime * deltaTime));
}
