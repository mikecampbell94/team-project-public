#pragma once

#include <functional>

#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Matrix4.h"
#include "../Utilities/Maths/Matrix3.h"
#include "../Utilities/Maths/Quaternion.h"

#include "CollisionShape.h"


class PhysicsNode;

typedef std::function<bool(PhysicsNode* this_obj, PhysicsNode* colliding_obj)> PhysicsCollisionCallback;
typedef std::function<void(const Matrix4& transform)> PhysicsUpdateCallback;

class GameObject;

class PhysicsNode
{
public:
	PhysicsNode()
		: position(0.0f, 0.0f, 0.0f)
		, linVelocity(0.0f, 0.0f, 0.0f)
		, force(0.0f, 0.0f, 0.0f)
		, invMass(0.0f)
		, orientation(0.0f, 0.0f, 0.0f, 1.0f)
		, angVelocity(0.0f, 0.0f, 0.0f)
		, torque(0.0f, 0.0f, 0.0f)
		, invInertia(Matrix3::ZeroMatrix)
		, collisionShape(NULL)
		, friction(0.5f)
		, elasticity(0.9f)
	{
	}
	~PhysicsNode()
	{
		delete collisionShape;
		collisionShape = nullptr;
	};


	Vector3 getPosition();
	void setPosition(Vector3 pos);

private:
	GameObject*				parent;
	Matrix4					worldTransform;
	PhysicsUpdateCallback	onUpdateCallback;


	Vector3		position;
	Vector3		linVelocity;
	Vector3		force;
	float		invMass;

	Quaternion  orientation;
	Vector3		angVelocity;
	Vector3		torque;
	Matrix3     invInertia;

	CollisionShape*		collisionShape;
	PhysicsCollisionCallback	onCollisionCallback;

	float	elasticity;
	float	friction;
};

