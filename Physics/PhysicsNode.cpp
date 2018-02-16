#include "PhysicseNode.h"



PhysicsNode::PhysicsNode()
{
}


PhysicsNode::~PhysicsNode()
{
}

Vector3 PhysicsNode::getPosition()
{
	return position;
}

void PhysicsNode::setPosition(Vector3 pos)
{
	this->position = pos;
}
