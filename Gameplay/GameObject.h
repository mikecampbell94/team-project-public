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
	void setPosition(NCLVector3 position);
	void setRotation(NCLVector4 rotation);
	void setScale(NCLVector3 scale);
	void setEnabled(bool isEnabled);

	const NCLVector3& getScale() const
	{
		return scale;
	}

	bool getEnabled()
	{
		return isEnabled;
	}

	bool isEnabled = true;
private: 
	SceneNode *sceneNode;
	PhysicsNode *physicsNode = nullptr;

	NCLVector3 position;
	NCLVector3 scale;

};

