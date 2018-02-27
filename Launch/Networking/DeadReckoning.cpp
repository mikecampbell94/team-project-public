#include "DeadReckoning.h"
#include "../Physics/PhysicsNode.h"

void DeadReckoning::blendStates(PhysicsNode* node, float factor)
{
	node->setPosition(interpolate(node->getPosition(), prediction.position, factor));

	node->setLinearVelocity(interpolate(node->startVelocity, prediction.linearVelocity, factor));

	node->setAcceleration(interpolate(node->startAcceleration, prediction.linearAcceleration, factor));
}

void DeadReckoning::predictPosition(float deltaTime)
{
	prediction.position += (prediction.linearVelocity * deltaTime) + ((prediction.linearAcceleration * 0.5f) * (deltaTime * deltaTime));
}

Vector3 DeadReckoning::interpolate(Vector3 a, Vector3 b, float factor)
{
	return a + ((b - a) * factor);
}
