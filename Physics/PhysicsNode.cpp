#include "PhysicsNode.h"

static const Vector3 gravity = Vector3(0.0f, -9.81f, 0.0f);
static const float dampingFactor = 0.999f;

void PhysicsNode::integrateForVelocity(float dt)
{
	if (invMass > 0.f) 
	{
		linVelocity += gravity * dt;
	}

	linVelocity += force * invMass * dt;

	linVelocity = linVelocity * dampingFactor;

	angVelocity += invInertia * torque * dt;

	angVelocity = angVelocity * dampingFactor;
}

void PhysicsNode::integrateForPosition(float dt)
{
	position += linVelocity * dt;

	orientation = orientation + Quaternion(angVelocity * dt * .5f, 0.f) * orientation;

	orientation.normalise();

	fireOnUpdateCallback();
}
