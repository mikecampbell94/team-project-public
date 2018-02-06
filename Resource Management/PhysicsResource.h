#pragma once
#include "ResourceClass.h"

class PhysicsResource : public Resourceclass
{
public:
	PhysicsResource() {};
	~PhysicsResource() {};
protected:
	Matrix4	    worldTransform;
	Vector3		position;
	//PhysicsUpdateCallback	onUpdateCallback;
	Vector3		linVelocity;
	Vector3		force;
	float		invMass;
	//Quaternion  orientation;
	Vector3		angVelocity;
	Vector3		torque;
	Matrix3     invInertia;
	//PhysicsCollisionCallback	onCollisionCallback;
	float				elasticity;		

};

