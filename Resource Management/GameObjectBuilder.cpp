#include "GameObjectBuilder.h"

GameObjectBuilder::GameObjectBuilder()
{

}

GameObjectBuilder::~GameObjectBuilder()
{
}

GameObject * GameObjectBuilder::buildGameObject(Node * node, Database* database)
{
	SceneNode* sceneNode = buildSceneNode(node->children[0], database);
	GameObject* gameObject = new GameObject();
	
	gameObject->setSize(sizeof(GameObject));
	gameObject->setName(node->name);
	gameObject->setSceneNode(sceneNode);
	
	gameObject->stats.colourToPaint = buildVector4(node->children[0]->children[1]);

	gameObject->setScale(buildVector3(node->children[4]));

	if (node->children.size() >= 6)
	{

		PhysicsNode* physicsNode = buildPhysicsNode(node->children[5], gameObject);
		gameObject->setPhysicsNode(physicsNode);
	}

	gameObject->setPosition(buildVector3(node->children[2]));
	gameObject->setRotation(buildVector4(node->children[3]));

	return gameObject;
}

SceneNode * GameObjectBuilder::buildSceneNode(Node * node, Database* database)
{
	std::string meshName = node->children[0]->value;
	SceneNode* sceneNode = new SceneNode(static_cast<Mesh*>(database->getTable("Meshes")->getResource(meshName)));
	sceneNode->SetColour(buildVector4(node->children[1]));

	if (node->children.size() > 2)
	{
		sceneNode->isReflective = true;
		sceneNode->reflectiveStrength = stof(node->children[2]->value);
	}

	return sceneNode;
}

PhysicsNode * GameObjectBuilder::buildPhysicsNode(Node * node, GameObject * parent)
{
	PhysicsNode* physicsnode = new PhysicsNode();

	physicsnode->setParent(parent);

	for (Node* child : node->children)
	{
		if (child->nodeType == "Enabled")
		{
			physicsnode->setEnabled(child->value == "True");
		}
		else if (child->nodeType == "TransmitCollision")
		{
			physicsnode->transmitCollision = child->value == "True";
		}
		else if (child->nodeType == "MultipleTransmitions")
		{
			physicsnode->multipleTransmitions = child->value == "True";
		}
		else if (child->nodeType == "CollisionShape")
		{
			physicsnode->setCollisionShape(child->value);
		}
		else if (child->nodeType == "Mass")
		{
			physicsnode->setInverseMass(stof(child->value));
			physicsnode->setInverseInertia(physicsnode->getCollisionShape()->buildInverseInertia(physicsnode->getInverseMass()));
		}
		else if (child->nodeType == "Elasticity")
		{
			physicsnode->setElasticity(stof(child->value));
		}
		else if (child->nodeType == "Friction")
		{
			physicsnode->setFriction(stof(child->value));
		}
		else if (child->nodeType == "Damping")
		{
			physicsnode->setDamping(stof(child->value));
		}
		else if (child->nodeType == "isStatic")
		{
			physicsnode->setStatic(child->value == "True");
		}
		else if (child->nodeType == "isCollision")
		{
			physicsnode->setIsCollision(child->value == "True");
		}
	}
	return physicsnode;
}

NCLVector3 GameObjectBuilder::buildVector3(Node * node)
{
	NCLVector3 vec;
	vec.x = stof(node->children[0]->value);
	vec.y = stof(node->children[1]->value);
	vec.z = stof(node->children[2]->value);
	return vec;
}

NCLVector4 GameObjectBuilder::buildVector4(Node * node)
{
	NCLVector4 vec;
	vec.x = stof(node->children[0]->value);
	vec.y = stof(node->children[1]->value);
	vec.z = stof(node->children[2]->value);
	vec.w = stof(node->children[3]->value);
	return vec;
}
