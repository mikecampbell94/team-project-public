#pragma once
#include "../Utilities/Maths/Vector3.h"


class CollisionShape;
class PhysicsNode
{
public:
	PhysicsNode();
	~PhysicsNode();

	
	Vector3 getPosition();
	void setPosition(Vector3 pos);
	inline float getMass()const { return Mass; }
	inline std::string getCollisionShape()const { return collisionShape; }
	bool getisCollision()const { return isCollision; }
	bool getEnablePhysics()const { return isphysics; }
	inline float getElasticity() { return elasticity; }
	inline float getFriction() { return friction; }

	

	inline void setMass(float v) { Mass = v; }
	inline void setCollisionShape(std::string colshape)
	{
			
	}
	void setisCollision(bool iscol) { isCollision = iscol; }
	void setEnablePhysics(bool isPhy) { isphysics = isPhy; }
	void setElasticity(float v) { elasticity = v; }
	void setFriction(float v) { friction = v; }

private:
	
	Vector3 position;
	float Mass;
	std::string collisionShape;
	bool isCollision;
	bool isphysics;
	float elasticity;
	float friction;
};
