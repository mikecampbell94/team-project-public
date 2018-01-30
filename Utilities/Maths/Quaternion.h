/******************************************************************************
Class:Quaternion
Implements:
Author:Rich Davison
Description:VERY simple Quaternion class. Students are encouraged to modify 
this as necessary...if they dare.

Quaternions aren't really discussed much in the graphics module, but the 
MD5Mesh class uses them internally, as MD5 files store their rotations
as quaternions.

I hate Quaternions.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include "MathsCommon.h"
#include "Matrix4.h"

class Matrix4;

class Quaternion	{
public:
	Quaternion(void);
	Quaternion(float x, float y, float z, float w);

	~Quaternion(void);

	float x;
	float y;
	float z;
	float w;

	void	normalise();
	Matrix4 toMatrix() const;


	Quaternion	conjugate() const;
	void		generateW();	//builds 4th component when loading in shortened, 3 component quaternions

	static Quaternion eulerAnglesToQuaternion(float pitch, float yaw, float roll);
	static Quaternion axisAngleToQuaterion(const Vector3& vector, float degrees);

	static void rotatePointByQuaternion(const Quaternion &q, Vector3 &point);

	static Quaternion fromMatrix(const Matrix4 &m);

	static float dot(const Quaternion &a, const Quaternion &b);

	Quaternion operator *(const Quaternion &a) const;
	Quaternion operator *(const Vector3 &a) const;

	inline friend std::ostream& operator<<(std::ostream& o, const Quaternion& q){
		o << "Quat(" << q.x << "," << q.y << "," << q.z <<  "," << q.w << ")" << std::endl;
		return o;
	}
};