#pragma once

#include "../Utilities/Maths/Matrix4.h"
#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Vector4.h"
#include "../Meshes/Mesh.h"
#include <vector>

class SceneNode
{
public:
	SceneNode(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	virtual ~SceneNode(void);

	void  SetTransform(const Matrix4 &matrix) { transform = matrix; }
	void	SetTransform(Vector3 pos) {
		transform.setPositionVector(pos);
	}

	const Matrix4&	GetTransform()		const { return transform; }
	Matrix4			GetWorldTransform() const { return worldTransform; }

	Vector4			GetColour()			const { return colour; }
	void			SetColour(Vector4 c) { colour = c; }

	Vector3			GetModelScale()		const { return modelScale; }
	void			SetModelScale(Vector3 s) { modelScale = s; }

	Mesh*			GetMesh()			const { return mesh; }
	void			SetMesh(Mesh* m) { mesh = m; }

	float	GetBoundingRadius()			const { return boundingRadius; }
	void	SetBoundingRadius(float f) { boundingRadius = f; }
	void	AutoSetBoundingRadius() {
		float a = max(modelScale.x, modelScale.y);
		float b = max(a, modelScale.z);

		boundingRadius = b;
	}

	float	GetCameraDistance()			const { return distanceFromCamera; }
	void	SetCameraDistance(float f) { distanceFromCamera = f; }

	void			AddChild(SceneNode* s);
	void			RemoveChild(SceneNode* s);

	virtual void	Update(float msec);
	virtual void	Draw(Shader& shader);
	virtual void	DrawShadow();

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart() {
		return children.begin();
	}

	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd() {
		return children.end();
	}

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera)
			? true : false;
	}

protected:
	SceneNode*	parent;
	Mesh*		mesh;
	Matrix4		worldTransform;
	Matrix4		transform;
	Vector3		modelScale;
	Vector4		colour;

	float		distanceFromCamera;
	float		boundingRadius;

	std::vector<SceneNode*> children;
};

