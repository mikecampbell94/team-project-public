#include "TableCreation.h"
#include "Table.h"
#include "../Resources/Resource.h"
#include "../../Graphics/Meshes/Mesh.h"
#include "../../Gameplay/GameObject.h"
#include "../../Graphics/Scene Management/SceneNode.h"

const size_t MAX_MEMORY_PER_TABLE = 5000;

TableCreation::TableCreation(Database* database)
{
	this->database = database;

	tableAdditions.push_back(std::bind(&TableCreation::addGameObject, this));
	tableAdditions.push_back(std::bind(&TableCreation::addPhysicsObject, this));
	tableAdditions.push_back(std::bind(&TableCreation::addMesh, this));

	addTablesToDatabase();
}

TableCreation::~TableCreation()
{
}

void TableCreation::addTablesToDatabase() const
{
	for each (auto addTableToDatabase in tableAdditions)
	{
		addTableToDatabase();
	}
}

void TableCreation::addGameObject() const
{
	
	database->addTable("GameObjects", new Table<Resource>("GameObjects", MAX_MEMORY_PER_TABLE, [&](Node* node)
	{
		std::string meshName = node->children[0]->value;
		Vector3 position;
		position.x = std::stof(node->children[2]->children[0]->value);
		position.y = std::stof(node->children[2]->children[1]->value);
		position.z = std::stof(node->children[2]->children[2]->value);
		Vector4 rotation;
		rotation.x = std::stof(node->children[3]->children[0]->value);
		rotation.y = std::stof(node->children[3]->children[1]->value);
		rotation.z = std::stof(node->children[3]->children[2]->value);
		rotation.w = std::stof(node->children[3]->children[3]->value);
		Vector3 scale;
		scale.x = std::stof(node->children[4]->children[0]->value);
		scale.y = std::stof(node->children[4]->children[1]->value);
		scale.z = std::stof(node->children[4]->children[2]->value);
		Vector4 colour;
		colour.x = std::stof(node->children[5]->children[0]->value);
		colour.y = std::stof(node->children[5]->children[1]->value);
		colour.z = std::stof(node->children[5]->children[2]->value);
		colour.w = std::stof(node->children[5]->children[3]->value);

		SceneNode* sceneNode = new SceneNode(static_cast<Mesh*>(database->getTable("Meshes")->getAllResources()->getResource(meshName)));
		sceneNode->SetColour(colour);
		GameObject* gameObject = new GameObject();
		gameObject->setName(node->name);
		gameObject->setSceneNode(sceneNode);
		gameObject->setPosition(position);
		gameObject->setScale(scale);
		return gameObject;
	}));
}

void TableCreation::addPhysicsObject() const
{
	database->addTable("PhysicsObjects", new Table<Resource>("PhysicsObjects", MAX_MEMORY_PER_TABLE, [](Node* node)
	{
		//Build object from node
		return nullptr;
	}));
}

void TableCreation::addMesh() const
{
	database->addTable("Meshes", new Table<Resource>("Meshes", MAX_MEMORY_PER_TABLE, [](Node* node)
	{
		Mesh* mesh = new Mesh(node->children[0]->value,1);
		mesh->loadTexture(node->children[1]->value);
		mesh->setName(node->name);
		return mesh;
	}));
}

