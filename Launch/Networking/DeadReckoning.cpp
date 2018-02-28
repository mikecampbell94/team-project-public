#include "DeadReckoning.h"
#include "../Physics/PhysicsNode.h"

void DeadReckoning::blendStates(PhysicsNode* node, float factor)
{
	node->setPosition(interpolate(node->getPosition(), prediction.position, factor));
}

void DeadReckoning::predictPosition(float deltaTime)
{
	prediction.linearVelocity += prediction.linearAcceleration * deltaTime;
	prediction.position += prediction.linearVelocity * deltaTime;
}

Vector3 DeadReckoning::interpolate(Vector3 a, Vector3 b, float factor)
{
	return a + ((b - a) * factor);
}
