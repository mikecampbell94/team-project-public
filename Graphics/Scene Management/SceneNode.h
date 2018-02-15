#pragma once

#include "../Utilities/Maths/Matrix4.h"
#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Vector4.h"
#include "../Meshes/Mesh.h"
#include <vector>

class SceneNode
{
public:
	SceneNode(string meshFile, Vector4 colour = Vector4(1, 1, 1, 1));
	SceneNode(Mesh* mesh, Vector4 colour = Vector4(1, 1, 1, 1));
	virtual ~SceneNode(void);

	void  SetTransform(const Matrix4 &matrix)
	{
		transform = matrix;
	}

	void	SetTransform(Vector3 pos) 
	{
		transform.setPositionVector(pos);
	}

	Mesh* GetMesh()
	{
		return mesh;
	}

	Matrix4 GetTransform()
	{
		return transform;
	}
	Matrix4 GetWorldTransform() const
	{
		return worldTransform;
	}

	void SetColour(Vector4 c)
	{
		this->colour = c;
		mesh->SetbackupColourAttributeForAllSubMeshes(c);
	}

	Vector4 getColour();

	void SetModelScale(Vector3 s)
	{
		boundingRadius *= s.length();
		transform.setScalingVector(s);
	}

	float GetCameraDistance() const
	{
		return distanceFromCamera;
	}
	void	SetCameraDistance(float f) { distanceFromCamera = f; }

	void AddChild(SceneNode* s);
	void RemoveChild(SceneNode* s);

	virtual void Update(float msec);
	virtual void Draw(Shader& shader);
	virtual void DrawShadow();

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

	std::vector<SceneNode*> getChildren();


	const float getRadius();

protected:
	bool		enabled;
	SceneNode*	parent;
	Mesh*		mesh;
	Matrix4		worldTransform;
	Matrix4		transform;
	Vector4		colour;

	float		distanceFromCamera;
	float		boundingRadius;

	std::vector<SceneNode*> children;
};

