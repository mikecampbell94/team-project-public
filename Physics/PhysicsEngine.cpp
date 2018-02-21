#include "PhysicsEngine.h"
#include "CollisionDetectionSAT.h"


PhysicsEngine::PhysicsEngine() : Subsystem("Physics")
{
	std::vector<MessageType> types = { MessageType::TEXT, MessageType::PLAYER_INPUT, MessageType::RELATIVE_TRANSFORM };

	incomingMessages = MessageProcessor(types, DeliverySystem::getPostman()->getDeliveryPoint("Physics"));

	updateTimestep = 1.0f / 60.f;
	updateRealTimeAccum = 0.0f;
}


PhysicsEngine::~PhysicsEngine()
{
	removeAllPhysicsObjects();
}

void PhysicsEngine::addPhysicsObject(PhysicsNode * obj)
{
	physicsNodes.push_back(obj);
}

void PhysicsEngine::removePhysicsObject(PhysicsNode * obj)
{
	//Lookup the object in question
	auto found_loc = std::find(physicsNodes.begin(), physicsNodes.end(), obj);

	//If found, remove it from the list
	if (found_loc != physicsNodes.end())
	{
		physicsNodes.erase(found_loc);
	}
}

void PhysicsEngine::removeAllPhysicsObjects()
{
	for (Constraint* c : constraints)
	{
		delete c;
	}
	constraints.clear();

	for (Manifold* m : manifolds)
	{
		delete m;
	}
	manifolds.clear();


	for (PhysicsNode* obj : physicsNodes)
	{
		if (obj->getParent())
			obj->getParent()->setPhysicsNode(nullptr);
		delete obj;
	}
	physicsNodes.clear();
}

void PhysicsEngine::updateSubsystem(const float& deltaTime)
{
	const int maxUpdatesPerFrame = 5;
	
	updateRealTimeAccum += deltaTime;
	for (int i = 0; (updateRealTimeAccum >= updateTimestep) && i < maxUpdatesPerFrame; ++i)
	{
		updateRealTimeAccum -= updateTimestep;

		updatePhysics();
	}

	if (updateRealTimeAccum >= updateTimestep)
	{
		updateRealTimeAccum = 0.0f;
	}
}

void PhysicsEngine::updatePhysics()
{
	for (Manifold* m : manifolds)
	{
		delete m;
	}
	manifolds.clear();

	
	//A whole physics engine in 6 simple steps =D

	//-- Using positions from last frame --
	//1. Broadphase Collision Detection (Fast and dirty)
	broadPhaseCollisions();
	
	//2. Narrowphase Collision Detection (Accurate but slow)
	narrowPhaseCollisions();
	
	//3. Initialize Constraint Params (precompute elasticity/baumgarte factor etc)	
	for (Manifold* m : manifolds)
	{
		m->preSolverStep(updateTimestep);
	}
	for (Constraint* c : constraints)
	{
		c->preSolverStep(updateTimestep);
	}


	//4. Update Velocities
	for (PhysicsNode* obj : physicsNodes) 
	{
		obj->integrateForVelocity(updateTimestep);
	}
	
	//5. Constraint Solver
	for (size_t i = 0; i < SOLVER_ITERATIONS; ++i) 
	{
		for (Manifold* m : manifolds) m->applyImpulse();
		for (Constraint* c : constraints) c->applyImpulse(updateTimestep);
	}
	
	//6. Update Positions (with final 'real' velocities)
	for (PhysicsNode* obj : physicsNodes)
	{
		obj->integrateForPosition(updateTimestep);
	}
	
}

void PhysicsEngine::broadPhaseCollisions()
{
	broadphaseColPairs.clear();

	PhysicsNode *pnodeA, *pnodeB;

	if (physicsNodes.size() > 0)
	{
		for (size_t i = 0; i < physicsNodes.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < physicsNodes.size(); ++j)
			{
				pnodeA = physicsNodes[i];
				pnodeB = physicsNodes[j];
				
				if (pnodeA->getCollisionShape() != NULL
					&& pnodeB->getCollisionShape() != NULL)
				{
					CollisionPair cp;
					cp.pObjectA = pnodeA;
					cp.pObjectB = pnodeB;

					broadphaseColPairs.push_back(cp);

				}
			}
		}
	}
}

void PhysicsEngine::narrowPhaseCollisions()
{
	if (broadphaseColPairs.size() > 0)
	{
		CollisionData colData;

		CollisionDetectionSAT colDetect;

		for (size_t i = 0; i < broadphaseColPairs.size(); ++i)
		{
			CollisionPair& cp = broadphaseColPairs[i];

			CollisionShape *shapeA = cp.pObjectA->getCollisionShape();
			CollisionShape *shapeB = cp.pObjectB->getCollisionShape();

			colDetect.beginNewPair(
				cp.pObjectA,
				cp.pObjectB,
				cp.pObjectA->getCollisionShape(),
				cp.pObjectB->getCollisionShape());

			
			if (colDetect.areColliding(&colData))
			{
				bool okA = cp.pObjectA->fireOnCollisionEvent(cp.pObjectA, cp.pObjectB);
				bool okB = cp.pObjectB->fireOnCollisionEvent(cp.pObjectB, cp.pObjectA);

				if (okA && okB)
				{
					Manifold* manifold = new Manifold();
					manifold->initiate(cp.pObjectA, cp.pObjectB);

					colDetect.genContactPoints(manifold);

					if (manifold->contactPoints.size() > 0) 
					{
						manifolds.push_back(manifold);
					}
					else {
						delete manifold;
					}
				}
			}
		}

	}
}
