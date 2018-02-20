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
	if (node->children.size() >= 6)
	{
		PhysicsNode* physicsNode = buildPhysicsNode(node->children[5]);
	}
	GameObject* gameObject = new GameObject();
	gameObject->setSize(sizeof(GameObject));
	gameObject->setName(node->name);
	gameObject->setSceneNode(sceneNode);
	gameObject->setPosition(buildVector3(node->children[2]));
	gameObject->setScale(buildVector3(node->children[4]));
	return gameObject;
}

SceneNode * GameObjectBuilder::buildSceneNode(Node * node, Database* database)
{
	std::string meshName = node->children[0]->value;
	SceneNode* sceneNode = new SceneNode(static_cast<Mesh*>(database->getTable("Meshes")->getResource(meshName)));
	sceneNode->SetColour(buildVector4(node->children[1]));
	return sceneNode;
}

PhysicsNode * GameObjectBuilder::buildPhysicsNode(Node * node)
{
	//PhysicsNode* physicsnode = new PhysicsNode();

	//std::string physicsEnabled = node->children[0]->value;

	//if (physicsEnabled == "True")
	//{
	//	physicsnode->setEnablePhysics(true);
	//}
	//else
	//{
	//	physicsnode->setEnablePhysics(false);
	//}
	//std::string CollisionEnabled = node->children[3]->value;
	//if (CollisionEnabled == "True")
	//{
	//	physicsnode->setisCollision(true);
	//}
	//else
	//{
	//	physicsnode->setisCollision(false);
	//}
	//physicsnode->setCollisionShape(node->children[1]->value);
	//physicsnode->setMass(stof(node->children[2]->value));	
	//physicsnode->setElasticity(stof(node->children[4]->value));
	//physicsnode->setFriction(stof(node->children[5]->value));
	return nullptr;
}

Vector3 & GameObjectBuilder::buildVector3(Node * node)
{
	Vector3 vec;
	vec.x = stof(node->children[0]->value);
	vec.y = stof(node->children[1]->value);
	vec.z = stof(node->children[2]->value);
	return vec;
}

Vector4 & GameObjectBuilder::buildVector4(Node * node)
{
	Vector4 vec;
	vec.x = stof(node->children[0]->value);
	vec.y = stof(node->children[1]->value);
	vec.z = stof(node->children[2]->value);
	vec.w = stof(node->children[3]->value);
	return vec;
}
