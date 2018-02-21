#pragma once

#include <functional>

#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Matrix4.h"
#include "../Utilities/Maths/Matrix3.h"
#include "../Utilities/Maths/Quaternion.h"
#include "../Gameplay/GameObject.h"

#include "CollisionShape.h"
#include "SphereCollisionShape.h"
#include "CuboidCollisionShape.h"


class PhysicsNode;

typedef std::function<bool(PhysicsNode* this_obj, PhysicsNode* colliding_obj)> PhysicsCollisionCallback;
typedef std::function<void(const Matrix4& transform)> PhysicsUpdateCallback;

//class GameObject;

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
		, enabled(true)
		, isCollision(true)
	{
	}
	~PhysicsNode()
	{
		delete collisionShape;
		collisionShape = nullptr;
	};

	void integrateForVelocity(float dt);
	void integrateForPosition(float dt);

	inline GameObject* getParent() const 
	{ 
		return parent; 
	}
	inline float getElasticity() const 
	{ 
		return elasticity; 
	}
	inline float getFriction() const 
	{ 
		return friction; 
	}
	inline const Vector3& getPosition() const 
	{ 
		return position; 
	}
	inline const Vector3& getLinearVelocity() const 
	{ 
		return linVelocity; 
	}
	inline const Vector3& getForce() const 
	{ 
		return force; 
	}
	inline float getInverseMass() const 
	{ 
		return invMass; 
	}
	inline const Quaternion& getOrientation() const 
	{ 
		return orientation; 
	}
	inline const Vector3& getAngularVelocity() const 
	{ 
		return angVelocity; 
	}
	inline const Vector3& getTorque() const 
	{ 
		return torque; 
	}
	inline const Matrix3& getInverseInertia() const 
	{ 
		return invInertia; 
	}
	inline CollisionShape* getCollisionShape() const 
	{ 
		return collisionShape; 
	}
	const Matrix4& getWorldSpaceTransform() const 
	{ 
		return worldTransform; 
	}
	inline const bool getEnabled() const
	{
		return enabled;
	}
	inline const bool getIsCollision() const
	{
		return isCollision;
	}



	inline void setParent(GameObject* obj) 
	{ 
		parent = obj; 
	}
	inline void setElasticity(float elasticityCoeff) 
	{ 
		elasticity = elasticityCoeff; 
	}
	inline void setFriction(float frictionCoeff) 
	{ 
		friction = frictionCoeff; 
	}

	inline void setPosition(const Vector3& v) 
	{ 
		position = v; 
		
		worldTransform = orientation.toMatrix();

		worldTransform.setPositionVector(position);
	}
	inline void setLinearVelocity(const Vector3& v) 
	{ 
		linVelocity = v; 
	}

	inline void setForce(const Vector3& v)
	{
		force = v;
	}

	inline void applyForce(const Vector3& v)
	{
		force += v;

		//linVelocity += (v * invMass);
		//angVelocity += invInertia * Vector3::cross(worldTransform.getPositionVector(), v);
	}
	
	inline void setInverseMass(const float& v) 
	{ 
		invMass = v; 
	}

	inline void setOrientation(const Quaternion& v) 
	{ 
		orientation = v; 
		
		worldTransform = orientation.toMatrix();

		worldTransform.setPositionVector(position);
	}
	inline void setAngularVelocity(const Vector3& v) 
	{ 
		angVelocity = v; 
	}
	inline void setTorque(const Vector3& v) 
	{ 
		torque = v; 
	}
	inline void setInverseInertia(const Matrix3& v) 
	{ 
		invInertia = v; 
	}

	inline void setCollisionShape(std::string colshape)
	{
		CollisionShape* colShape;
		
		if (colshape == "Sphere")
		{
			colShape = new SphereCollisionShape(parent->getScale().x);
		} 
		else if (colshape == "Box")
		{
			colShape = new CuboidCollisionShape(parent->getScale());
		}
		else
		{
			colShape = nullptr;
		}
		
		if (collisionShape) collisionShape->setParent(NULL);
		collisionShape = colShape;
		if (collisionShape) collisionShape->setParent(this);
	}
	inline void setEnabled(bool isPhy)
	{
		enabled = isPhy;
	}
	inline void setIsCollision(bool isCol)
	{
		isCollision = isCol;
	}


	inline void setOnCollisionCallback(PhysicsCollisionCallback callback) 
	{ 
		onCollisionCallback = callback; 
	}
	inline bool fireOnCollisionEvent(PhysicsNode* obj_a, PhysicsNode* obj_b)
	{
		return (onCollisionCallback) ? onCollisionCallback(obj_a, obj_b) : true;
	}

	inline void setOnUpdateCallback(PhysicsUpdateCallback callback)
	{
		onUpdateCallbacks.push_back(callback);
	}

	inline void fireOnUpdateCallback()
	{
		if (worldTransform.getPositionVector() != previousTransform.getPositionVector())
		{
			for each (PhysicsUpdateCallback callback in onUpdateCallbacks)
			{
				if (callback) callback(worldTransform);
			}

			movedSinceLastBroadPhase = true;
		}

		previousTransform = worldTransform;
	}

	bool toDeleteInOctree = false;
	bool movedSinceLastBroadPhase = false;

private:
	GameObject*				parent;
	Matrix4					worldTransform;
	Matrix4 previousTransform;
	PhysicsUpdateCallback	onUpdateCallback;
	std::vector<PhysicsUpdateCallback>	onUpdateCallbacks;


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

	bool enabled;
	bool isCollision;
};

