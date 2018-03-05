#pragma once
#include "CollisionShape.h"
#include "Hull.h"

class CuboidCollisionShape :
	public CollisionShape
{
public:
	CuboidCollisionShape();
	CuboidCollisionShape(const Vector3& halfdims);
	~CuboidCollisionShape();

	void setScale(Vector3 scale, float invMass) override
	{
		halfDims = scale;
		buildInverseInertia(invMass);
	}

	void setHalfWidth(float half_width) 
	{ 
		halfDims.x = fabs(half_width); 
	}
	void setHalfHeight(float half_height) 
	{ 
		halfDims.y = fabs(half_height); 
	}
	void setHalfDepth(float half_depth) 
	{ 
		halfDims.z = fabs(half_depth); 
	}

	const Vector3& getHalfDims() const 
	{ 
		return halfDims; 
	}
	float getHalfWidth()	const 
	{ 
		return halfDims.x; 
	}
	float getHalfHeight()	const 
	{ 
		return halfDims.y; 
	}
	float getHalfDepth()	const 
	{ 
		return halfDims.z; 
	}


	virtual Matrix3 buildInverseInertia(float invMass) const override;

	virtual void getCollisionAxes(const PhysicsNode* otherObject, std::vector<Vector3>& out_axes) const override;

	virtual Vector3 getClosestPoint(const Vector3& point) const override;

	virtual void getMinMaxVertexOnAxis(const Vector3& axis, Vector3& out_min, Vector3& out_max) const override;

	virtual void getIncidentReferencePolygon(const Vector3& axis, std::list<Vector3>& out_face, Vector3& out_normal, std::vector<Plane>& out_adjacent_planes) const override;

private:
	static void constructCubeHull();

	Vector3 halfDims;
	static Hull cubeHull;
};

