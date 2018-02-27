#include "PhysicsNode.h"
#include "../Communication/Messages/MoveCameraRelativeToGameObjectMessage.h"
#include "../Communication/DeliverySystem.h"

static const Vector3 gravity = Vector3(0.0f, -9.8f, 0.0f);
static const float MAX_SPEED = 50.0f;

void PhysicsNode::integrateForVelocity(float dt)
{
	if (invMass > 0.f) 
	{
		appliedForce += gravity;

		force = appliedForce;
	}

	linVelocity += force * invMass * dt;

	linVelocity = linVelocity * damping;

	if(linVelocity.length() > MAX_SPEED)
	{
		linVelocity = linVelocity.normalise() * MAX_SPEED;
	}

	angVelocity += invInertia * torque * dt;

	angVelocity = angVelocity * damping;

	appliedForce.toZero();
}

void PhysicsNode::integrateForPosition(float dt)
{
	position += linVelocity * dt;

	orientation = orientation + Quaternion(angVelocity * dt * .5f, 0.f) * orientation;

	orientation.normalise();

	worldTransform = orientation.toMatrix();

	worldTransform.setPositionVector(position);
	fireOnUpdateCallback();

	//if (parent->getName() == MoveCameraRelativeToGameObjectMessage::resourceName)
	//{
	//	DeliverySystem::getPostman()->insertMessage(UpdatePositionMessage("NetworkClient", parent->getName(), position));
	//}
}
