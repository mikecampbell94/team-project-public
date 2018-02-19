#pragma once

#include "PhysicsNode.h";
#include "../Utilities/Maths/Vector3.h"

class Constraint
{
public:
	Constraint() {};
	~Constraint() {};

	virtual void applyImpulse(float dt) = 0;

	virtual void preSolverStep(float dt) {}
};

