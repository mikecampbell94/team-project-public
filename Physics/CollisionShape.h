#pragma once

//#include "GeometryUtils.h"
#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Plane.h"
#include "../Utilities/Maths/Matrix3.h"
#include <vector>
#include <list>

//using namespace GeometryUtils;

class PhysicsNode;

struct CollisionEdge
{
	CollisionEdge(const Vector3& a, const Vector3& b)
		: _v0(a), _v1(b) {}

	Vector3 _v0;
	Vector3 _v1;
};

class CollisionShape
{
public:
	CollisionShape() : m_Parent(NULL) {}
	virtual ~CollisionShape() {}

	virtual Matrix3 buildInverseInertia(float invMass) const = 0;

	inline void setParent(PhysicsNode* node) 
	{ 
		m_Parent = node; 
	}
	inline		 PhysicsNode* parent() 
	{ 
		return m_Parent; 
	}
	inline const PhysicsNode* parent() const 
	{ 
		return m_Parent; 
	}

	virtual void getCollisionAxes(
		const PhysicsNode* otherObject,
		std::vector<Vector3>& out_axes) const = 0;

	virtual Vector3 getClosestPoint(const Vector3& point) const = 0;

	virtual void getMinMaxVertexOnAxis(
		const Vector3& axis,
		Vector3& out_min,
		Vector3& out_max) const = 0;

	virtual void getIncidentReferencePolygon(
		const Vector3& axis,
		std::list<Vector3>& out_face,
		Vector3& out_normal,
		std::vector<Plane>& out_adjacent_planes) const = 0;

protected:
	PhysicsNode* m_Parent;
};