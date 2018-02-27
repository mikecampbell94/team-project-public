#include "DeadReckoning.h"
#include "../Physics/PhysicsNode.h"

//KinematicState DeadReckoning::predictState(PhysicsNode* node, float deltaTime)
//{
//	//KinematicState newState = oldState;
//	
//	//newState.position = oldState.position + (oldState.linearVelocity * deltaTime)
//	//	+ ((oldState.linearAcceleration * 0.5f) * (deltaTime * deltaTime));
//}
//
//KinematicState DeadReckoning::blendStates(KinematicState oldState, KinematicState newState, float blendFactor)
//{
//}
//void DeadReckoning::predictPosition(PhysicsNode* node, float deltaTime)
//{
//	float dt = 1.0f / 60.f;
//		node->setPosition(node->getPosition() + (node->getLinearVelocity() * dt)
//			+ ((node->getAcceleration() * 0.5f) * (dt * dt)));
//}
void DeadReckoning::blendStates(PhysicsNode* node, KinematicState newState, float factor)
{
	float oldFactor = 1.0f - factor;

	//node->setPosition((node->getPosition() * oldFactor) + (newState.position * factor));
	node->setPosition(interpolate(node->getPosition(), newState.position, factor));

	//node->setLinearVelocity((node->getLinearVelocity() * oldFactor) + (newState.linearVelocity * factor));
	node->setLinearVelocity(interpolate(node->getLinearVelocity(), newState.linearVelocity, factor));

	//node->setAcceleration((node->getAcceleration() * oldFactor) + (newState.linearAcceleration * factor));
	node->setAcceleration(interpolate(node->getAcceleration(), newState.linearAcceleration, factor));
}

Vector3 DeadReckoning::interpolate(Vector3 a, Vector3 b, float factor)
{
	return a + ((b - a) * factor);
}
