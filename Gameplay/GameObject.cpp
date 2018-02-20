#include "GameObject.h"

#include "../Physics/PhysicsNode.h"

GameObject::GameObject()
{
	setSize(sizeof(*this));
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

void GameObject::updatePosition()
{
	Vector3 newPosition = this->physicsNode->getWorldSpaceTransform().getPositionVector();
	Vector3 scale = this->sceneNode->GetWorldTransform().getScalingVector();

	Matrix4 newTransform = this->physicsNode->getWorldSpaceTransform();// *Matrix4::scale(scale);
	newTransform.setScalingVector(scale);
	this->sceneNode->SetTransform(newPosition);
}

void GameObject::setPosition(Vector3 position)
{
	this->position = position;
	this->sceneNode->SetTransform(position);
	if(this->physicsNode != nullptr)
		this->physicsNode->setPosition(position);
}

void GameObject::setRotation()
{
}

void GameObject::setScale(Vector3 scale)
{
	this->scale = scale;
	this->sceneNode->SetModelScale(scale);
}


