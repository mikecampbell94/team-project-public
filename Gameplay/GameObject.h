#pragma once
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Physics/PhysicseNode.h"
#include "../Resource Management/Resources/Resource.h"

class GameObject : public Resource
{
public:
	GameObject();
	~GameObject();

	void setSceneNode(SceneNode* sceneNode);
	void setPhysicsNode(PhysicsNode* physicsNode);

	SceneNode* getSceneNode();
	PhysicsNode* getPhysicsNode();

private: 
	SceneNode *sceneNode;
	PhysicsNode *physicsNode;

};

