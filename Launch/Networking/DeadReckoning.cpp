#include "DeadReckoning.h"
#include "../Physics/PhysicsNode.h"

void DeadReckoning::blendStates(PhysicsNode* node, KinematicState newState, float factor)
{
	node->setPosition(interpolate(node->startPosition, newState.position, factor));

	node->setLinearVelocity(interpolate(node->startVelocity, newState.linearVelocity, factor));

	node->setAcceleration(interpolate(node->startAcceleration, newState.linearAcceleration, factor));
}

Vector3 DeadReckoning::interpolate(Vector3 a, Vector3 b, float factor)
{
	return a + ((b - a) * factor);
}
