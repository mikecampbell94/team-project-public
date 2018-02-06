#pragma once
#include "ResourceClass.h"

typedef unsigned int GLuint;

class RenderResource: public Resourceclass
{
public:
	RenderResource() {};
	~RenderResource() {};
	inline void  SetMesh(GameObject* p->Render()->GetMesh()) { mesh = p->Render()->GetMesh(); }
	//void  SetworldTransform(const GameObject* p->Render()->GetWorldTransform()) { worldTransform = p->Render()->GetWorldTransform(); }
	inline void  SetTransform(const GameObject* p->Render()->GetTransform()) { transform = p->Render()->GetTransform(); }
	inline void  SetColour(GameObject* p->Render()->GetColor()) { colour = p->Render()->GetColor(); }
	inline void  SetCameraDistance(GameObject* p->Render()->GetCameraDistance()) { distanceFromCamera = p->Render()->GetCameraDistance(); }
	inline void  SetBoundingRadius(GameObject* p->Render()->GetBoundingRadius()) { boundingRadius = p->Render()->GetBoundingRadius(); }
	inline void  SetTextureName(GameObject* p->Render()->GetMesh()->GetTexture()) { texturename = p->Render()->GetMesh()->GetTexture(); }
protected:
	Mesh*  mesh;
	//Matrix4	 worldTransform;
	Matrix4	 transform;
	Vector3	 modelScale;
	Vector4	 colour;	
	float	distanceFromCamera;
	float	boundingRadius;
	GLuint  texturename;

};


