#include "DeadReckoning.h"
#include "../Physics/PhysicsNode.h"

void DeadReckoning::blendStates(PhysicsNode* node, KinematicState newState, float factor)
{
	node->setPosition(interpolate(node->getPosition(), newState.position, factor));

	node->setLinearVelocity(interpolate(node->getLinearVelocity(), newState.linearVelocity, factor));

	node->setAcceleration(interpolate(node->getAcceleration(), newState.linearAcceleration, factor));
}

Vector3 DeadReckoning::interpolate(Vector3 a, Vector3 b, float factor)
{
	return a + ((b - a) * factor);
}
