#pragma once

#include "Constraint.h"
#include "PhysicsEngine.h"

class DistanceConstraint : public Constraint
{
public:
	DistanceConstraint(PhysicsNode* obj1, PhysicsNode* obj2,
		const Vector3& globalOnA, const Vector3& globalOnB)
	{
		pnodeA = obj1;
		pnodeB = obj2;

		Vector3 ab = globalOnB - globalOnA;
		targetLength = ab.length();

		Vector3 r1 = (globalOnA - pnodeA->getPosition());
		Vector3 r2 = (globalOnB - pnodeB->getPosition());
		relPosA = Matrix3::Transpose(pnodeA->getOrientation().toMatrix3()) * r1;
		relPosB = Matrix3::Transpose(pnodeB->getOrientation().toMatrix3()) * r2;
	}

	virtual void applyImpulse(float dt) override
	{
		Vector3 r1 = pnodeA->getOrientation().toMatrix3() * relPosA;
		Vector3 r2 = pnodeB->getOrientation().toMatrix3() * relPosB;

		Vector3 globalOnA = r1 + pnodeA->getPosition();
		Vector3 globalOnB = r2 + pnodeB->getPosition();

		Vector3 ab = globalOnB - globalOnA;
		Vector3 abn = ab;
		abn.normalise();

		Vector3 v0 = pnodeA->getLinearVelocity() + Vector3::cross(pnodeA->getAngularVelocity(), r1);
		Vector3 v1 = pnodeB->getLinearVelocity() + Vector3::cross(pnodeB->getAngularVelocity(), r2);

		float abnVel = Vector3::dot(v0 - v1, abn);

		float invConstraintMassLin = pnodeA->getInverseMass() + pnodeB->getInverseMass();

		float invConstraintMassRot = Vector3::dot(abn,
			Vector3::cross(pnodeA->getInverseInertia() * Vector3::cross(r1, abn), r1) +
			Vector3::cross(pnodeB->getInverseInertia() * Vector3::cross(r2, abn), r2));

		float constraintMass = invConstraintMassLin + invConstraintMassRot;

		if (constraintMass > 0.0f) {
			float b = 0.0f;

			float distance_offset = ab.length() - targetLength;
			float baumgarte_scalar = 0.1f;
			b = -(baumgarte_scalar / dt) * distance_offset;

			float jn = -(abnVel + b) / constraintMass;

			pnodeA->setLinearVelocity(pnodeA->getLinearVelocity() + abn*(pnodeA->getInverseMass() * jn));
			pnodeB->setLinearVelocity(pnodeB->getLinearVelocity() - abn*(pnodeB->getInverseMass() * jn));

			pnodeA->setAngularVelocity(pnodeA->getAngularVelocity() + pnodeA->getInverseInertia() * Vector3::cross(r1, abn * jn));
			pnodeB->setAngularVelocity(pnodeB->getAngularVelocity() - pnodeB->getInverseInertia() * Vector3::cross(r2, abn * jn));


		}
	}



protected:
	PhysicsNode *pnodeA, *pnodeB;

	float   targetLength;

	Vector3 relPosA;
	Vector3 relPosB;
};