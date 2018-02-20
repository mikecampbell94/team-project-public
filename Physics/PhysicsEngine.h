#pragma once

#include "../Launch/Systems/Subsystem.h"

#include "PhysicsNode.h"
#include "Constraint.h"
#include "Manifold.h"
#include <vector>



//Number of jacobi iterations to apply in order to
// assure the constraints are solved. (Last tutorial)
#define SOLVER_ITERATIONS 50


struct CollisionPair
{
	PhysicsNode* pObjectA;
	PhysicsNode* pObjectB;
};

class PhysicsEngine : public Subsystem
{
public:
	
	PhysicsEngine();
	~PhysicsEngine();

	void addPhysicsObject(PhysicsNode* obj);
	void removePhysicsObject(PhysicsNode* obj);
	void removeAllPhysicsObjects();

	void addConstraint(Constraint* c) { constraints.push_back(c); }


	void updateSubsystem(const float& deltaTime) override;


	inline float getUpdateTimestep() const 
	{ 
		return updateTimestep; 
	}
	inline void setUpdateTimestep(float updateTimestep) 
	{ 
		updateTimestep = updateTimestep; 
	}

	inline const Vector3& getGravity() const 
	{ 
		return gravity; 
	}
	inline void setGravity(const Vector3& g) 
	{ 
		gravity = g; 
	}

	inline float getDampingFactor() const 
	{ 
		return dampingFactor; 
	}
	inline void  setDampingFactor(float d) 
	{ 
		dampingFactor = d; 
	}

	inline float getDeltaTime() const { 
		return updateTimestep; 
	}


private:
	void updatePhysics();

	void broadPhaseCollisions();

	void narrowPhaseCollisions();


	float		updateTimestep, updateRealTimeAccum;
	
	Vector3		gravity;
	float		dampingFactor;


	std::vector<CollisionPair>  broadphaseColPairs;

	std::vector<PhysicsNode*>	physicsNodes;

	std::vector<Constraint*>	constraints;
	std::vector<Manifold*>		manifolds;	

};