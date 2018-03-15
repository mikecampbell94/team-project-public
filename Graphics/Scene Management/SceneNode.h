#pragma once

#include "../Utilities/Maths/Matrix4.h"
#include "../Utilities/Maths/Vector3.h"
#include "../Utilities/Maths/Vector4.h"
#include "../Meshes/Mesh.h"
#include <vector>

class GameObject;

class SceneNode
{
public:
	SceneNode(string meshFile, NCLVector4 colour = NCLVector4(1, 1, 1, 1));
	SceneNode(Mesh* mesh, NCLVector4 colour = NCLVector4(1, 1, 1, 1));
	virtual ~SceneNode(void);

	void setParent(GameObject* parentObject)
	{
		this->parentObject = parentObject;
	}

	GameObject* getParent()
	{
		return parentObject;
	}

	void setPosition(NCLVector3 position)
	{
		transform.setPositionVector(position);
	}

	void  setTransform(const NCLMatrix4 &matrix)
	{
		transform = matrix;
	}

	void	setTransform(NCLVector3 pos) 
	{
		transform.setPositionVector(pos);
	}

	Mesh* getMesh()
	{
		return mesh;
	}

	NCLMatrix4 getTransform()
	{
		return transform;
	}
	NCLMatrix4 getWorldTransform() const
	{
		return worldTransform;
	}

	void setColour(NCLVector4 c)
	{
		this->colour = c;
		mesh->setbackupColourAttributeForAllSubMeshes(c);
	}

	NCLVector4 getColour();

	void setModelScale(NCLVector3 s)
	{
		boundingRadius *= s.length();
		transform.setScalingVector(s);
	}

	float getCameraDistance() const
	{
		return distanceFromCamera;
	}
	void	setCameraDistance(float f) { distanceFromCamera = f; }

	void addChild(SceneNode* s);
	void removeChild(SceneNode* s);

	virtual void update(float msec);
	virtual void draw(Shader& shader);
	virtual void drawShadow(Shader& shader);

	std::vector<SceneNode*>::const_iterator getChildIteratorStart() {
		return children.begin();
	}

	std::vector<SceneNode*>::const_iterator getChildIteratorEnd() {
		return children.end();
	}

	static bool compareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera < b->distanceFromCamera)
			? true : false;
	}

	void setEnabled(bool isEnabled)
	{
		this->isEnabled = isEnabled;
	}

	std::vector<SceneNode*> getChildren();

	const float getRadius();

	bool isPaintSurface = false;
	int hasTexture = 0;
	bool isEnabled = true;
	NCLVector4 axisAngleRotation;

	bool isReflective = false;
	float reflectiveStrength = 0.0f;

protected:
	
	SceneNode*	parent;
	Mesh*		mesh;
	NCLMatrix4		worldTransform;
	NCLMatrix4		transform;
	NCLVector4		colour;

	float		distanceFromCamera;
	float		boundingRadius;

	std::vector<SceneNode*> children;
	GameObject* parentObject;
};

