#include "SphereCollisionShape.h"
#include "PhysicsNode.h"



SphereCollisionShape::SphereCollisionShape()
{
	radius = 1.f;
}

SphereCollisionShape::SphereCollisionShape(float newRadius)
{
	radius = newRadius;
}


SphereCollisionShape::~SphereCollisionShape()
{
}

Matrix3 SphereCollisionShape::buildInverseInertia(float invMass) const
{
	float i = 2.5f * invMass / (radius * radius);

	Matrix3 inertia;
	inertia._11 = i;
	inertia._22 = i;
	inertia._33 = i;

	return inertia;
}

void SphereCollisionShape::getCollisionAxes(const PhysicsNode * otherObject, std::vector<Vector3>& out_axes) const
{
	Vector3 dir = (otherObject->getPosition() - parent()->getPosition()).normalise();

	Vector3 p1 = parent()->getPosition();
	Vector3 p2 = otherObject->getCollisionShape()->getClosestPoint(p1);

	out_axes.push_back((p1 - p2).normalise());
}

Vector3 SphereCollisionShape::getClosestPoint(const Vector3 & point) const
{
	Vector3 diff = (point - parent()->getPosition()).normalise();
	return parent()->getPosition() + diff * radius;
}

void SphereCollisionShape::getMinMaxVertexOnAxis(const Vector3 & axis, Vector3 & out_min, Vector3 & out_max) const
{
	out_min = parent()->getPosition() - axis * radius;
	out_max = parent()->getPosition() + axis * radius;
}

void SphereCollisionShape::getIncidentReferencePolygon(const Vector3 & axis, std::list<Vector3>& out_face, Vector3 & out_normal, std::vector<Plane>& out_adjacent_planes) const
{
	out_face.push_back(parent()->getPosition() + axis * radius);
	out_normal = axis;
}
