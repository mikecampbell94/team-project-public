#include "TableCreation.h"

#include "Table.h"
#include "../Resources/Resource.h"

#include "../../Audio/Sound.h"

const size_t MAX_MEMORY_PER_TABLE = 5000;

TableCreation::TableCreation(Database* database)
{
	this->database = database;

	tableAdditions.push_back(std::bind(&TableCreation::addGameObject, this));
	tableAdditions.push_back(std::bind(&TableCreation::addSceneNode, this));
	tableAdditions.push_back(std::bind(&TableCreation::addPhysicsObject, this));
	tableAdditions.push_back(std::bind(&TableCreation::addSounds, this));
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
	database->addTable("GameObjects", new Table<Resource>("GameObjects", MAX_MEMORY_PER_TABLE, [](Node* node)
	{
		//Build object from node
		return nullptr;
	}));
}

void TableCreation::addSceneNode() const
{
	database->addTable("SceneNodes", new Table<Resource>("SceneNodes", MAX_MEMORY_PER_TABLE, [](Node* node)
	{
		//Build object from node
		return nullptr;
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

void TableCreation::addSounds() const
{
	database->addTable("SoundObjects", new Table<Resource>("SoundObjectes", MAX_MEMORY_PER_TABLE, [](Node* node)
	{
		Sound *sound = new Sound(node->value);
		sound->setName(node->name);
		return sound;
	}));
}
