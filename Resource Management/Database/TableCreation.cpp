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
		SceneNode* sceneNode = new SceneNode(static_cast<Mesh*>(database->getTable("Meshes")->getAllResources()->getResource("SphereMesh")));
		GameObject* gameObject = new GameObject();
		gameObject->setName(node->name);
		gameObject->setSceneNode(sceneNode);
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
		Mesh* mesh = new Mesh(node->value,1);
		mesh->setName(node->name);
		return mesh;
	}));
}

