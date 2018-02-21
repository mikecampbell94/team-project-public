#pragma once
/*
	AUTHOR: RICH DAVISON
*/


#include "Vector3.h"

class Plane
{
public:
	Plane(void) {};
	Plane(const Vector3 &normal, float distance, bool normalise = false, Vector3 pos = Vector3());
	Plane(const Vector3 &normal, Vector3 pos, bool normalise = false);
	~Plane(void) {};

	void	setNormal(const Vector3 &normal)	{ this->normal = normal; }
	Vector3 getNormal()					const	{ return normal; }

	void	setDistance(float dist)				{ distance = dist; }
	float	getDistance()				const	{ return distance; }

	bool sphereInPlane(const Vector3 &position, float radius)		const;
	bool sphereOutsidePlane(const Vector3& position, float radius)	const;
	bool sphereIntersecting(const Vector3& position, float radius)	const;
	bool pointInPlane(const Vector3& position) const;

	Vector3 position;
protected:
	Vector3 normal;
	float distance;

};

