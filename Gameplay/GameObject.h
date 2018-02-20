#pragma once
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Resource Management/Resources/Resource.h"
#include "../Utilities/Maths/Vector3.h"

class PhysicsNode;

class GameObject : public Resource
{
public:
	GameObject();
	~GameObject();

	void setSceneNode(SceneNode* sceneNode);
	void setPhysicsNode(PhysicsNode* physicsNode);

	SceneNode* getSceneNode();
	PhysicsNode* getPhysicsNode();



	void updatePosition();
	void setPosition(Vector3 position);
	void setRotation();
	void setScale(Vector3 scale);

	const Vector3& getScale() const
	{
		return scale;
	}

private: 
	SceneNode *sceneNode;
	PhysicsNode *physicsNode = nullptr;

	Vector3 position;
	Vector3 scale;
};

