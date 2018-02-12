#pragma once
#include "../Graphics/Scene Management/SceneNode.h"
#include "../Physics/PhysicseNode.h"


class GameObject
{
public:
	GameObject();
	~GameObject();



private: 
	SceneNode *sceneNode;
	PhysicsNode *physicsNode;

};

