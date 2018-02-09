#pragma once

#include "../../Utilities/Maths/Matrix4.h"
#include "../../Utilities/Maths/Vector3.h"
#include "Frustum.h"

class SubMesh;

class Camera
{
public:
	Camera(void) {
		yaw = 0.0f;
		pitch = 0.0f;
	};

	Camera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
	}

	~Camera(void) {
	};

	void updateCamera(float msec = 10.0f);

	bool subMeshIsInCameraView(SubMesh* submesh);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	Matrix4 buildViewMatrix();
	void updateViewFrustum(Matrix4& projectionMatrix);

	//Gets position in world space
	Vector3 getPosition() const {
		return position;
	}

	//Sets position in world space
	void setPosition(Vector3 val)
	{
		position = val;

	}

	//Gets yaw, in degrees
	float	getYaw()   const { return yaw; }
	//Sets yaw, in degrees
	void	setYaw(float y) { yaw = y; }

	//Gets pitch, in degrees
	float	getPitch() const { return pitch; }
	//Sets pitch, in degrees
	void	setPitch(float p) { pitch = p; }

	Matrix4 viewMatrix;

protected:
	Frustum viewFrustum;

	Vector3 position;
	float	yaw;
	float	pitch;
};

