#include "PhysicsNode.h"



PhysicsNode::PhysicsNode()
{
}


PhysicsNode::~PhysicsNode()
{
}

void PhysicsNode::integrateForVelocity(float dt)
{
	linVelocity += force * invMass * dt;

	angVelocity += invInertia * torque * dt;
}

void PhysicsNode::integrateForPosition(float dt)
{
	position += linVelocity * dt;

	orientation = orientation + Quaternion(angVelocity * dt * .5f, 0.f) * orientation;

	orientation.normalise();

	fireOnUpdateCallback();
}
