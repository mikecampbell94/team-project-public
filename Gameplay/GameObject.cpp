#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::setSceneNode(SceneNode * sceneNode)
{
	this->sceneNode = sceneNode;
}

void GameObject::setPhysicsNode(PhysicsNode * physicsNode)
{
	this->physicsNode = physicsNode;
}

SceneNode * GameObject::getSceneNode()
{
	return sceneNode;
}

PhysicsNode * GameObject::getPhysicsNode()
{
	return physicsNode;
}


