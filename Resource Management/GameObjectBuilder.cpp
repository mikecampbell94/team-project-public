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
	gameObject->setPosition(buildVector3(node->children[2]));
	gameObject->setScale(buildVector3(node->children[4]));
	return gameObject;
}

SceneNode * GameObjectBuilder::buildSceneNode(Node * node, Database* database)
{
	std::string meshName = node->value;
	SceneNode* sceneNode = new SceneNode(static_cast<Mesh*>(database->getTable("Meshes")->getResource(meshName)));
	return sceneNode;
}

PhysicsNode * GameObjectBuilder::buildPhysicsNode(Node * node)
{
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
	vec.z = stof(node->children[2]->value);
	return vec;
}
