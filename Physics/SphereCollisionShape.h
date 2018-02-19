#pragma once
#include "CollisionShape.h"
class SphereCollisionShape : public CollisionShape
{
public:
	SphereCollisionShape();
	SphereCollisionShape(float newRadius);
	~SphereCollisionShape();

	void setRadius(float newRadius)
	{
		radius = newRadius;
	}
	float getRadius()
	{
		return radius;
	}

	virtual Matrix3 buildInverseInertia(float invMass) const override;

	virtual void getCollisionAxes(const PhysicsNode* otherObject, std::vector<Vector3>& out_axes) const override;

	virtual Vector3 getClosestPoint(const Vector3& point) const override;

	virtual void getMinMaxVertexOnAxis(const Vector3& axis, Vector3& out_min, Vector3& out_max) const override;

	virtual void getIncidentReferencePolygon(const Vector3& axis, std::list<Vector3>& out_face, Vector3& out_normal, std::vector<Plane>& out_adjacent_planes) const override;

private:
	float radius;
};

