#pragma once
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Physics/PhysicseNode.h"
#include "../Resource Management/Resources/Resource.h"
#include "../Utilities/Maths/Vector3.h"


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

private: 
	SceneNode *sceneNode;
	PhysicsNode *physicsNode;

	Vector3 position;

};

