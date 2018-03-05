#include "GameObject.h"

#include "../Physics/PhysicsNode.h"

GameObject::GameObject()
{
	setSize(sizeof(*this));
	canJump = false;
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
	Matrix4 newTransform = this->physicsNode->getWorldSpaceTransform();
	newTransform = newTransform * Matrix4::scale(scale);

	this->sceneNode->SetTransform(newTransform);
}

void GameObject::setPosition(Vector3 position)
{
	this->position = position;
	this->sceneNode->SetTransform(position);

	if(this->physicsNode != nullptr)
	{
		this->physicsNode->setPosition(position);
	}
}

void GameObject::setRotation(Vector4 rotation)
{
	Vector3 position = sceneNode->GetTransform().getPositionVector();
	Vector3 scale = sceneNode->GetTransform().getScalingVector();

	this->sceneNode->SetTransform(Matrix4::translation(position) * 
		Matrix4::rotation(rotation.w, Vector3(rotation.x, rotation.y, rotation.z)) * 
		Matrix4::scale(scale));
	
	if (this->physicsNode != nullptr)
	{
		this->physicsNode->setRotation(rotation);
	}
}

void GameObject::setScale(Vector3 scale)
{
	this->scale = scale;
	this->sceneNode->SetModelScale(scale);
}


