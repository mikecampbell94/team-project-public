#pragma once

#include "../Utilities/Maths/Plane.h"
#include "../Utilities/Maths/Matrix4.h"

class Matrix4;

class Frustum
{
public:
	Frustum() {}
	~Frustum() {}

	void fromMatrix(const Matrix4 &mvp);
	bool insideFrustum(const Vector3& position, const float& radius);

protected:
	Plane planes[6];
};